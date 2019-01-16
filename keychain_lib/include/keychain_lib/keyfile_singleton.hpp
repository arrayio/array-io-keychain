//
// Created by roman on 5/1/19.
//

#ifndef KEYCHAINAPP_KEYFILE_SINGLETON_HPP
#define KEYCHAINAPP_KEYFILE_SINGLETON_HPP

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/identity.hpp>

#include "keyfile_parser.hpp"

namespace keychain_app {

using keyfile_map_2_t = boost::multi_index::multi_index_container<
  keyfile_format::keyfile_t,
  boost::multi_index::indexed_by<
    boost::multi_index::ordered_unique<boost::multi_index::const_mem_fun<keyfile_format::keyfile_t,const dev::Public&,&keyfile_format::keyfile_t::public_key> >,
    boost::multi_index::ordered_non_unique<boost::multi_index::member<keyfile_format::keyfile_t,std::string,&keyfile_format::keyfile_t::keyname> >,
    boost::multi_index::sequenced<>
  >
>;

using keyfile_map_t = std::map<dev::Public, keyfile_format::keyfile_t>;

class keyfile_singleton
{
  keyfile_singleton();
  ~keyfile_singleton();
  
  using value_t = keyfile_map_t::value_type;
  keyfile_map_t m_keydata_map;
  using prim_key_type = keyfile_map_t::key_type;
public:
  //TODO: need to add access for second index (keyname)
  using iterator = keyfile_map_t::iterator;
  using const_iterator = keyfile_map_t::const_iterator;
  static keyfile_singleton& instance();
  const_iterator begin() const;
  const_iterator end() const;
  keyfile_format::keyfile_t& operator[](const prim_key_type& key);
  void insert(keyfile_format::keyfile_t&& keyfile_data); //NOTE: keyfile_t (fc_light::variant) is not support move semantic
  void update(keyfile_format::keyfile_t&& keyfile_data);
  bool is_exist(const prim_key_type& key) const;
  void flush_keyfile(const prim_key_type& key) const;
  void flush_all() const;
};

}



#endif //KEYCHAINAPP_KEYFILE_SINGLETON_HPP

