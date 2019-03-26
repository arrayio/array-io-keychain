//
// Created by user on 3/15/19.
//

#ifndef KEYCHAINAPP_KEYDATA_SINGLETON_HPP
#define KEYCHAINAPP_KEYDATA_SINGLETON_HPP

#include <eth-crypto/crypto/Common.h>
#include <fc_light/reflect/reflect.hpp>
#include <fc_light/reflect/variant.hpp>
#include <fc_light/exception/exception.hpp>
#include <fc_light/io/json.hpp>
#include <boost/hana.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/for_each.hpp>
#include <boost/hana/size.hpp>
#include "keyfile_singleton.hpp"

namespace keychain_app {

namespace keydata
{

std::string seed(dev::bytes& );
void derive_masterkey(std::string&, std::string&);
void derive_key(std::string&, std::string& );
void restore(const char *, std::string&, std::string& );
void backup(const char * );
std::pair<dev::Secret, dev::bytes> get_master_key(get_password_create_f&& );
std::vector<char> pbkdf2(std::string const& _pass);


struct path_levels_t
{
    path_levels_t(){}

    std::string root;
    int purpose;
    int coin_type;
    int account;
    int change;
    int address_index;
};

struct backup_t
{
    backup_t(){}
    backup_t(std::string &keyname_, fc_light::variant path_): keyname(keyname_), path(path_){}
    std::string keyname;
    fc_light::variant path;
};

enum struct levels_te
{
    null,
    root,
    purpose,
    coin_type,
    account,
    change,
    address_index,
    last
};

struct create_t
{
    create_t(){};
    std::string keyname;
    std::string description;
    bool encrypted;
    keyfile_format::cipher_etype cipher;
    keyfile_format::curve_etype curve;
    std::string password;
    fc_light::variant path;
};

namespace hana = boost::hana;
constexpr auto level_static_list =
        hana::make_range(
                hana::int_c<static_cast<int>(levels_te::purpose)>,
                hana::int_c<static_cast<int>(levels_te::last)>);
}

}

FC_LIGHT_REFLECT_ENUM(
        keychain_app::keydata::levels_te,
        (null)
        (root)
        (purpose)
        (coin_type)
        (account)
        (change)
        (address_index)
        (last)
)

FC_LIGHT_REFLECT(keychain_app::keydata::path_levels_t, (root)(purpose)(coin_type)(account)(change)(address_index))
FC_LIGHT_REFLECT(keychain_app::keydata::create_t,  (keyname)(description)(encrypted)(cipher)(curve)(password)(path))
FC_LIGHT_REFLECT(keychain_app::keydata::backup_t,  (keyname)(path))

#endif //KEYCHAINAPP_KEYDATA_SINGLETON_HPP
