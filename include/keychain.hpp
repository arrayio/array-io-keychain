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
#include <graphene/utilities/key_conversion.hpp>
#include <json/json.hpp>

#include <boost/filesystem.hpp>

#include "sign_define.hpp"

namespace keychain_app
{

using secp256_private_key = fc::ecc::private_key;
static const char* KEY_DEFAULT_PATH = "./key_data";

namespace json_parser {

namespace json_file_keys
{
static const char* TYPE = "type";
static const char* USERNAME = "username";
static const char* CURVE_TYPE = "curve_type";
static const char* ENCRYPTED = "encrypted";
static const char* KEY_DATA_FORMAT = "key_data_format";
static const char* KEY_DATA = "key_data";

namespace values
{
static const char* KEY = "key";
static const char* SECP256 = "secp256k1";
static const char* WIF_FORMAT = "wif";
}

}

namespace json_keys
{

static const char* COMMAND = "command";
static const char* RESULT = "result";
static const char* ERROR = "error";
static const char* PARAMS = "params";

namespace sign_command
{
static const char* CMD_NAME = "sign";
namespace params
{
static const char* CHAINID = "chainid";
static const char* HDPATH = "hdpath";
static const char* TRANSACTION = "transaction";
static const char* KEYFILE = "keyfile";
static const char* KEYNAME = "keyname";
}
}
}
}

std::string to_hex(const uint8_t* data, size_t length);

namespace bfs = boost::filesystem;

class keychain
{
public:
  keychain(const char* default_key_dir = KEY_DEFAULT_PATH);
  void operator()(const nlohmann::json& message);

private:

  bfs::path m_default_path;

  void sign_command(const nlohmann::json& j_params);
  secp256_private_key get_priv_key_from_str(const std::string& str);
  nlohmann::json create_response(const signature_t& signature);
  fc::sha256 get_hash(const keychain_app::unit_list_t &list);
  static nlohmann::json open_keyfile(const char* filename);
};

}

#endif //KEYCHAINAPP_KEYCHAIN_HPP
