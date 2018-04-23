//
// Created by roman on 4/18/18.
//

#ifndef KEYCHAINAPP_KEY_FILE_PARSER_HPP
#define KEYCHAINAPP_KEY_FILE_PARSER_HPP

#include <fc/reflect/reflect.hpp>

namespace keychain_app {

namespace keyfile_format
{


enum file_type
{
    TYPE_UNKNOWN = 0,
    TYPE_KEY
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
    enum chipher_etype
    {
      CHIPHER_NONE = 0,
      CHIPHER_AES128,
      CHIPHER_AES192,
      CHIPHER_AES256
    } chipher_type;
    enum curve_etype
    {
      CURVE_UNKNOWN,
      CURVE_SECP256K1
    } curve_type;
    std::string data;
  } keyinfo;
};

}

}

FC_REFLECT_ENUM(keychain_app::keyfile_format::file_type, (TYPE_UNKNOWN)(TYPE_KEY))
FC_REFLECT_ENUM(keychain_app::keyfile_format::key_file::key_info::key_format, (FORMAT_UNKNOWN)(FORMAT_ARRAYIO))
FC_REFLECT_ENUM(keychain_app::keyfile_format::key_file::key_info::chipher_etype, (CHIPHER_NONE)(CHIPHER_AES128)(CHIPHER_AES192)(CHIPHER_AES256))
FC_REFLECT_ENUM(keychain_app::keyfile_format::key_file::key_info::curve_etype, (CURVE_UNKNOWN)(CURVE_SECP256K1))
FC_REFLECT(keychain_app::keyfile_format::key_file::key_info, (format)(chipher_type)(curve_type)(data))
FC_REFLECT(keychain_app::keyfile_format::key_file, (filetype)(username)(keyinfo))


#endif //KEYCHAINAPP_KEY_FILE_PARSER_HPP
