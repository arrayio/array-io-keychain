//
// Created by roman on 4/18/18.
//

#ifndef KEYCHAINAPP_KEY_FILE_PARSER_HPP
#define KEYCHAINAPP_KEY_FILE_PARSER_HPP

#include <fc/reflect/reflect.hpp>
#include <fc/variant.hpp>

namespace keychain_app {

namespace keyfile_format
{


enum file_type
{
    TYPE_UNKNOWN = 0,
    TYPE_KEY
};

enum cipher_etype
{
  CIPHER_UNKNOWN = 0,
  CIPHER_AES128,
  CIPHER_AES192,
  CIPHER_AES256
};

struct encrypted_data
{
  cipher_etype cipher_type;
  std::string iv;
  std::string enc_data;
};

struct key_file
{
  file_type filetype;
  std::string username;
  struct key_info
  {
    enum key_format
    {
      FORMAT_UNKNOWN = 0,
      FORMAT_ARRAYIO
    } format;
    bool encrypted;
    enum curve_etype
    {
      CURVE_UNKNOWN,
      CURVE_SECP256K1
    } curve_type;
    fc::variant data;//either std::string or encrypted_data
  } keyinfo;
};

}

}
FC_REFLECT_ENUM(keychain_app::keyfile_format::cipher_etype, (CIPHER_UNKNOWN)(CIPHER_AES128)(CIPHER_AES192)(CIPHER_AES256))
FC_REFLECT_ENUM(keychain_app::keyfile_format::file_type, (TYPE_UNKNOWN)(TYPE_KEY))
FC_REFLECT_ENUM(keychain_app::keyfile_format::key_file::key_info::key_format, (FORMAT_UNKNOWN)(FORMAT_ARRAYIO))
FC_REFLECT_ENUM(keychain_app::keyfile_format::key_file::key_info::curve_etype, (CURVE_UNKNOWN)(CURVE_SECP256K1))
FC_REFLECT(keychain_app::keyfile_format::encrypted_data, (cipher_type)(iv)(enc_data))
FC_REFLECT(keychain_app::keyfile_format::key_file::key_info, (format)(encrypted)(curve_type)(data))
FC_REFLECT(keychain_app::keyfile_format::key_file, (filetype)(username)(keyinfo))


#endif //KEYCHAINAPP_KEY_FILE_PARSER_HPP
