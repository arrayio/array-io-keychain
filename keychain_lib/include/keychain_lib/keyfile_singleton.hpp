//
// Created by roman on 5/1/19.
//

#pragma once

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/multi_index/identity.hpp>

#include "keyfile_parser.hpp"

namespace keychain_app {

namespace keyfiles_map {

struct prim_index_tag {};
struct second_index_tag {};

using keyfile_map_t = boost::multi_index::multi_index_container<
  keyfile_format::keyfile_t,
  boost::multi_index::indexed_by<
    boost::multi_index::ordered_unique<
      boost::multi_index::tag <prim_index_tag>,
      boost::multi_index::const_mem_fun<keyfile_format::keyfile_t, const dev::Public &, &keyfile_format::keyfile_t::public_key>
    >,
    boost::multi_index::ordered_non_unique<
      boost::multi_index::tag <second_index_tag>,
      boost::multi_index::member<keyfile_format::keyfile_t, std::string, &keyfile_format::keyfile_t::keyname>
    >,
   boost::multi_index::sequenced<>
  >
>;

}

using keyfiles_map::keyfile_map_t;

class keyfile_singleton
{
  keyfile_singleton();
  ~keyfile_singleton();
  
  using value_t = keyfile_map_t::value_type;
  keyfile_map_t m_keydata_map;
  using prim_key_type = keyfile_map_t::key_type;
  using second_index_type = keyfile_map_t::index<keyfiles_map::second_index_tag>::type;
  using second_key_type = second_index_type::key_type;
  
  void flush_keyfile_impl(const value_t& keyfile_data) const;
public:
  using iterator = keyfile_map_t::iterator;
  using const_iterator = keyfile_map_t::const_iterator;
  static keyfile_singleton& instance();
  const_iterator begin() const;
  const_iterator end() const;
  const second_index_type& second_index() const;
  const keyfile_format::keyfile_t& operator[](const prim_key_type& key) const;
  const keyfile_format::keyfile_t& operator[](const second_key_type& key) const;
  void insert(keyfile_format::keyfile_t&& keyfile_data); //NOTE: keyfile_t (fc_light::variant) is not support move semantic
  
  template <typename modifier_f>
  void update(const prim_key_type& key, modifier_f&& f)
  {
    auto it = m_keydata_map.find(key);
    if (it == m_keydata_map.end())
      FC_LIGHT_THROW_EXCEPTION(fc_light::key_not_found_exception, "Private key is not exist, public = ${key_}", ("key_", key));
    auto keyfile = *it;
    f(keyfile);
    auto res = m_keydata_map.replace(it, std::move(keyfile));
    if (!res)
      FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "Cannot replace keyfile in map, public = ${key_}", ("key_", key));
    flush_keyfile(key);
  };
  
  template <typename modifier_f>
  void update(const second_key_type& key, modifier_f&& f)
  {
    auto& ind = m_keydata_map.get<keyfiles_map::second_index_tag>();
    auto it = ind.find(key);
    if (it == ind.end())
      FC_LIGHT_THROW_EXCEPTION(fc_light::key_not_found_exception, "Private key is not exist, keyname = ${key_}", ("key_", key));
    auto keyfile = *it;
    f(keyfile);
    auto res = ind.replace(it, std::move(keyfile));
    if (!res)
      FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "Cannot replace keyfile in map, keyname = ${key_}", ("key_", key));
    flush_keyfile(key);
  };
  
  void update(keyfile_format::keyfile_t&& keyfile);
  
  bool is_exist(const prim_key_type& key) const;
  void flush_keyfile(const prim_key_type& key) const;
  void flush_keyfile(const second_key_type& key) const;
  void flush_all() const;
};

}