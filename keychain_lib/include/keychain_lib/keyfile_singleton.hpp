//
// Created by roman on 5/1/19.
//

#ifndef KEYCHAINAPP_KEYFILE_SINGLETON_HPP
#define KEYCHAINAPP_KEYFILE_SINGLETON_HPP

#include <map>
#include "key_file_parser.hpp"

namespace keychain_app {

using keyfile_map_t = std::map<std::string, keyfile_format::keyfile_t>;

class keyfile_singleton
{
  keyfile_singleton();
  ~keyfile_singleton();
  
  using value_t = keyfile_map_t::value_type;
  keyfile_map_t m_keydata_map;
public:
  using iterator = keyfile_map_t::iterator;
  static keyfile_singleton& instance();
  keyfile_format::keyfile_t& operator[](const std::string& key);
  void insert(keyfile_format::keyfile_t&& keyfile_data); //NOTE: keyfile_t (fc_light::variant) is not support move semantic
  void update(keyfile_format::keyfile_t&& keyfile_data);
  bool is_exist(const std::string& key) const;
  void flush_keyfile(const std::string& key) const;
  void flush_all() const;
};

}



#endif //KEYCHAINAPP_KEYFILE_SINGLETON_HPP

