//
// Created by roman on 5/1/19.
//

#ifndef KEYCHAINAPP_KEYFILE_SINGLETON_HPP
#define KEYCHAINAPP_KEYFILE_SINGLETON_HPP

#include <map>
#include "key_file_parser.hpp"

namespace keychain_app {

using keyfile_map_t = std::map<dev::Public, keyfile_format::keyfile_t>;

class keyfile_singleton
{
  keyfile_singleton();
  ~keyfile_singleton();
  
  using value_t = keyfile_map_t::value_type;
  keyfile_map_t m_keydata_map;
  using key_type = keyfile_map_t::key_type;
public:
  using iterator = keyfile_map_t::iterator;
  using const_iterator = keyfile_map_t::const_iterator;
  static keyfile_singleton& instance();
  const_iterator begin() const;
  const_iterator end() const;
  keyfile_format::keyfile_t& operator[](const key_type& key);
  void insert(keyfile_format::keyfile_t&& keyfile_data); //NOTE: keyfile_t (fc_light::variant) is not support move semantic
  void update(keyfile_format::keyfile_t&& keyfile_data);
  bool is_exist(const key_type& key) const;
  void flush_keyfile(const key_type& key) const;
  void flush_all() const;
};

}



#endif //KEYCHAINAPP_KEYFILE_SINGLETON_HPP

