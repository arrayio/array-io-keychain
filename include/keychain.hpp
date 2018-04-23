//
// Created by roman on 4/6/18.
//

#ifndef KEYCHAINAPP_KEYCHAIN_HPP
#define KEYCHAINAPP_KEYCHAIN_HPP

#include <functional>
#include <vector>
#include <iostream>
#include <fstream>

#include <fc/crypto/elliptic.hpp>
#include <fc/crypto/hex.hpp>
#include <fc/variant.hpp>
#include <graphene/utilities/key_conversion.hpp>
#include <json/json.hpp>

#include <boost/filesystem.hpp>

#include "keychain_commands.hpp"

namespace keychain_app
{

namespace bfs = boost::filesystem;

class keychain
{
public:
  keychain(const char* default_key_dir = KEY_DEFAULT_PATH);
  ~keychain();
  void operator()(const fc::variant& command);
private:
  bfs::path m_init_path;
};

struct keychain_commands_singletone
{
    using command_ptr = std::shared_ptr<keychain_command_base>;
    static const keychain_commands_singletone& instance();
    const command_ptr operator[](keychain_command_type cmd_type) const;
private:
    std::vector<command_ptr> m_command_list;
    keychain_commands_singletone();
};

}

#endif //KEYCHAINAPP_KEYCHAIN_HPP
