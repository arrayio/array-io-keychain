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

#include <fc_light/exception/exception.hpp>

#include "keyfile_parser.hpp"

namespace keychain_app {

namespace keyfiles_map {

struct prim_pubkey_tag {};
struct second_keyname_tag {};
struct third_date_tag {};
struct log_record_tag {};

using keyfile_map_t = boost::multi_index::multi_index_container<
  keychain_app::keyfile_format::keyfile_t,
  boost::multi_index::indexed_by<
    boost::multi_index::ordered_unique<
      boost::multi_index::tag <prim_pubkey_tag>,
      boost::multi_index::const_mem_fun<keyfile_format::keyfile_t, const dev::Public&, &keyfile_format::keyfile_t::public_key>
    >,
    boost::multi_index::ordered_non_unique<
      boost::multi_index::tag <second_keyname_tag>,
      boost::multi_index::member<keyfile_format::keyfile_t, std::string, &keyfile_format::keyfile_t::keyname>
    >,
    boost::multi_index::ordered_non_unique<
      boost::multi_index::tag <third_date_tag>,
      boost::multi_index::const_mem_fun<keyfile_format::keyfile_t, const fc_light::time_point&, &keyfile_format::keyfile_t::last_date>
    >
  >
>;

using log_records_t = boost::multi_index::multi_index_container<
  keyfile_format::log_record,
  boost::multi_index::indexed_by<
    boost::multi_index::ordered_non_unique<
      boost::multi_index::tag<log_record_tag>,
      boost::multi_index::member<keyfile_format::log_record, fc_light::time_point, &keyfile_format::log_record::sign_time>
    >
  >
>;

using signlog_map_t = std::map<dev::Public, log_records_t>;

}

using keyfiles_map::keyfile_map_t;
using keyfiles_map::signlog_map_t;
using keyfiles_map::log_records_t;

class keyfile_singleton
{
  keyfile_singleton();
  ~keyfile_singleton();
  
  using value_t = keyfile_map_t::value_type;
  keyfile_map_t m_keydata_map;
  signlog_map_t m_signlog_map;
  
  using prim_index_type = keyfile_map_t::index<keyfiles_map::prim_pubkey_tag>::type;
  using second_index_type = keyfile_map_t::index<keyfiles_map::second_keyname_tag>::type;
  using third_index_type = keyfile_map_t::index<keyfiles_map::third_date_tag>::type;
  using log_index_type = log_records_t::index<keyfiles_map::log_record_tag>::type;
  
  using prim_key_type = keyfile_map_t::key_type;
  using second_key_type = second_index_type::key_type;
  using third_key_type = third_index_type::key_type;
  
  void flush_keyfile_impl(const value_t& keyfile_data) const;
  void flush_logrecords_impl(const prim_key_type& key, const log_records_t& log_records) const;
  
  void keydata_initialize();
  void signlog_initialize();
public:
  static keyfile_singleton& instance();
  
  using iterator = keyfile_map_t::iterator; //primary_index
  using const_iterator = keyfile_map_t::const_iterator; //primary_index
  
  const log_index_type& get_logs(const dev::Public& pkey) const;
  void add_log_record(const dev::Public& pkey, const keyfile_format::log_record& record);
  
  const_iterator begin() const; //primary_index
  const_iterator end() const; //primary_index
  
  // NOTE: You can get all indexes
  // If you want to implement search in STL style you can get any index and call find function for every selected index
  // example code:
  // auto it = keyfile_map.second_index().find(keyname);
  // if (it == keyfile_map.second_index().end())
  //   return <- element not found result ->
  
  // For sorting record by key use begin/end function STL like way
  // auto it = keyfile_map.third_index().begin();
  // for (; it != keyfile_map.third_index().end(); ++it)
  // {
  //   const keyfile_t& key_file = *it;
  //   some_print_function(keyfile);
  // }
  const prim_index_type& prim_index() const;
  const second_index_type& second_index() const;
  const third_index_type& third_index() const;
  
  const keyfile_format::keyfile_t& operator[](const prim_key_type& key) const; //NOTE: use for searching by public key
  const keyfile_format::keyfile_t& operator[](const second_key_type& key) const; //NOTE: use for searchin by keyname
  void insert(keyfile_format::keyfile_t&& keyfile_data); //NOTE: keyfile_t (fc_light::variant) is not support move semantic
  
  template <typename modifier_f>
  void update(const prim_key_type& key, modifier_f&& f)
  {
    auto it = m_keydata_map.find(key);
    if (it == m_keydata_map.end())
      FC_LIGHT_THROW_EXCEPTION(fc_light::key_not_found_exception, "Private key is not exist, public = ${key_}", ("key_", key));
    auto keyfile = *it;
    f(keyfile);
    //NOTE: I don't want to use boost multi_index modify function because it is unsafe
    //If the rearrangement (reindexing) fails, the element is erased.
    //For example, modifier functor may modyfy prim unique key so that element will become non unique
    //I want to exclude a sequence where a user's private key can be deleted as a result of an internal keychain error
    auto res = m_keydata_map.replace(it, std::move(keyfile));
    if (!res)
      FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "Cannot replace keyfile in map, public = ${key_}", ("key_", key));
    flush_keyfile(key);
  };
  
  template <typename modifier_f>
  void update(const second_key_type& key, modifier_f&& f)
  {
    auto& ind = m_keydata_map.get<keyfiles_map::second_keyname_tag>();
    auto it = ind.find(key);
    if (it == ind.end())
      FC_LIGHT_THROW_EXCEPTION(fc_light::key_not_found_exception, "Private key is not exist, keyname = ${key_}", ("key_", key));
    auto keyfile = *it;
    f(keyfile);
    //NOTE: I don't want to use boost multi_index modify function because it is unsafe
    //If the rearrangement (reindexing) fails, the element is erased.
    //For example, modifier functor may modyfy prim unique key so that element will become non unique
    //I want to exclude a sequence where a user's private key can be deleted as a result of an internal keychain error
    auto res = ind.replace(it, std::move(keyfile));
    if (!res)
      FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "Cannot replace keyfile in map, keyname = ${key_}", ("key_", key));
    flush_keyfile(key);
  };
  
  void update(keyfile_format::keyfile_t&& keyfile);
  
  bool is_exist(const prim_key_type& key) const;
  void flush_keyfile(const prim_key_type& key) const;
  void flush_keyfile(const second_key_type& key) const;
  void flush_logrecords(const prim_key_type& key) const;
  void flush_all() const;
};

}