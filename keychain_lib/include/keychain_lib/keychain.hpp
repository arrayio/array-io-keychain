//
// Created by roman on 4/6/18.
//

#ifndef KEYCHAINAPP_KEYCHAIN_HPP
#define KEYCHAINAPP_KEYCHAIN_HPP

#include <functional>
#include <vector>
#include <iostream>
#include <fstream>

#include <fc_light/crypto/hex.hpp>
#include <fc_light/variant.hpp>

#include <boost/filesystem.hpp>
#include <boost/signals2.hpp>

#include "keychain_commands.hpp"

namespace keychain_app
{

class keychain_base;
namespace bfs = boost::filesystem;


class secure_dlg_mod_base
{
public:
    using string_list = std::list<std::wstring>;

    virtual ~secure_dlg_mod_base(){}
    
    virtual byte_seq_t get_passwd_trx(const std::string& json_cmd) const = 0;
    virtual byte_seq_t get_passwd_unlock(const std::string& keyname, int unlock_time) const = 0;
    virtual byte_seq_t get_passwd_on_create(const std::string& keyname = std::string("") ) const = 0;
    virtual void print_mnemonic(const string_list& mnemonic) const = 0;
    
    virtual void connect(keychain_base& keychain_) const;
};

class gui_mod_base
{
public:
  virtual ~gui_mod_base(){}
  
  virtual dev::Public select_key() const = 0;
  virtual void connect(keychain_base& keychain_) const;
};

class keychain : public keychain_base
{
public:
  static keychain& instance();
  virtual std::string operator()(const fc_light::variant& command) override;
private:
  keychain();
  virtual ~keychain();
  bfs::path m_init_path;
};

struct keychain_commands_singleton
{
    using command_ptr = std::shared_ptr<keychain_command_base>;
    static const keychain_commands_singleton& instance();
    const command_ptr operator[](command_te cmd_type) const;
private:
    std::vector<command_ptr> m_command_list;
    keychain_commands_singleton();
};

}

#endif //KEYCHAINAPP_KEYCHAIN_HPP
