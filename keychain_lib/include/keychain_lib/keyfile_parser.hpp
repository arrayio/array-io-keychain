//
// Created by roman on 4/18/18.
//

#ifndef KEYCHAINAPP_KEY_FILE_PARSER_HPP
#define KEYCHAINAPP_KEY_FILE_PARSER_HPP

#include <fc_light/variant.hpp>
#include <fc_light/time.hpp>

#include <eth-crypto/crypto/Common.h>
#include "sign_define.hpp"

namespace keychain_app {

namespace keyfile_format {

enum file_type {
  TYPE_UNKNOWN = 0,
  TYPE_KEY,
  TYPE_LOG
};

//for more information use "openssl enc -ciphers"
enum struct cipher_etype {
  unknown = 0,
  aes128,
  aes192,
  aes256
};

struct encrypted_data {
  cipher_etype cipher_type;
  std::string iv;
  std::string enc_data;
};

// for more information use "openssl ecparam -list_curves"
enum struct curve_etype {
  unknown,
//      secp224k1,
//      secp224r1,
    secp256k1
//      secp384r1,
//      secp521r1
};

struct keyfile_t {
  file_type filetype = TYPE_KEY;
  std::string keyname;
  std::string description;
  std::string keychain_version;
  fc_light::time_point creation_time;
  
  const dev::Public &public_key() const;
  
  const fc_light::time_point &last_date() const;//TODO: At this moment it returns creation date.
  // When sqlite will be utilized it will be possible to evaluate last usage date from adjacent table
  struct keyinfo_t {
    bool encrypted;
    curve_etype curve_type;
    fc_light::variant priv_key_data;//either std::string or encrypted_data
    dev::Public public_key;
  } keyinfo;
};

struct log_record
{
  log_record(const dev::bytes& transaction_, const fc_light::time_point& sign_time_, blockchain_te blockchain_type_, const std::string& chainid_)
    : transaction(transaction_)
    , sign_time(sign_time_)
    , blockchain_type(blockchain_type_)
    , chainid(chainid_) {}
  dev::bytes transaction;
  fc_light::time_point sign_time;
  blockchain_te blockchain_type;
  std::string chainid;
};

struct signlog_file_t
{
  file_type filetype = TYPE_LOG;
  dev::Public public_key;
  std::vector<log_record> sign_events;
};

}

}

FC_LIGHT_REFLECT_ENUM(keychain_app::keyfile_format::cipher_etype, (unknown)(aes128)(aes192)(aes256))
FC_LIGHT_REFLECT_ENUM(keychain_app::keyfile_format::file_type, (TYPE_UNKNOWN)(TYPE_KEY)(TYPE_LOG))
FC_LIGHT_REFLECT_ENUM(keychain_app::keyfile_format::curve_etype, (unknown)(secp256k1))
FC_LIGHT_REFLECT(keychain_app::keyfile_format::log_record, (transaction)(sign_time)(blockchain_type)(chainid))
FC_LIGHT_REFLECT(keychain_app::keyfile_format::signlog_file_t, (filetype)(public_key)(sign_events))
FC_LIGHT_REFLECT(keychain_app::keyfile_format::encrypted_data, (cipher_type)(iv)(enc_data))
FC_LIGHT_REFLECT(keychain_app::keyfile_format::keyfile_t::keyinfo_t, (encrypted)(curve_type)(priv_key_data)(public_key))
FC_LIGHT_REFLECT(keychain_app::keyfile_format::keyfile_t, (filetype)(keyname)(description)(keychain_version)(creation_time)(keyinfo))

#endif //KEYCHAINAPP_KEY_FILE_PARSER_HPP
