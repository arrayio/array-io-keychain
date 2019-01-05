//
// Created by roman on 5/1/19.
//

#ifndef KEYCHAINAPP_KEYFILE_SINGLETONE_HPP
#define KEYCHAINAPP_KEYFILE_SINGLETONE_HPP

#include <map>
#include "key_file_parser.hpp"

namespace keychain_app {

using keyfile_map_t = std::map<std::string, keyfile_format::keyfile_t>;

class keyfile_singleton: private keyfile_map_t
{
  using value_t = keyfile_map_t::value_type;
  keyfile_singleton();
  ~keyfile_singleton();
public:
  using iterator = keyfile_map_t::iterator;
  static const keyfile_singleton& instance();
  const keyfile_format::keyfile_t& operator[](const std::string& key) const;
  keyfile_format::keyfile_t& operator[](const std::string& key);
  void insert(keyfile_format::keyfile_t&& keyfile_data); //NOTE: keyfile_t (fc_light::variant) is not support move semantic
  bool is_exist(const std::string& key) const;
  void flush_keyfile(const std::string& key) const;
  void flush_all() const;
};

}



#endif //KEYCHAINAPP_KEYFILE_SINGLETONE_HPP

