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

#include "sign_define.hpp"

namespace keychain_app
{

using secp256_private_key = fc::ecc::private_key;

namespace json_parser
{
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
}
}
}
}

std::string to_hex(const uint8_t* data, size_t length);

class keychain
{
public:
  void operator()(const nlohmann::json& message);
  void sign_command(const nlohmann::json& j_params);
  secp256_private_key get_priv_key_from_file(const char* keyfile);

private:

  nlohmann::json create_response(const signature_t& signature);
  fc::sha256 get_hash(const keychain_app::unit_list_t &list);
};

}

#endif //KEYCHAINAPP_KEYCHAIN_HPP
