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

template <typename sign_creatorT>
class keychain : public  sign_creatorT
{
public:
  void operator()(const nlohmann::json& message) {
    if (!message.is_object())
      throw std::runtime_error("Error: json object excpected");
    const auto& command = message.at(json_parser::json_keys::COMMAND).get<std::string>();
    if (command == json_parser::json_keys::sign_command::CMD_NAME)
      sign_command(message.at(json_parser::json_keys::PARAMS));

  }

  void sign_command(const nlohmann::json& j_params)
  {
    unit_list_t unit_list;
    auto chainid_it = j_params.find(json_parser::json_keys::sign_command::params::CHAINID);
    if (chainid_it != j_params.end())
    {
      unit_list.push_back(fc::sha256(chainid_it->get<std::string>()));
    }
    const auto& transaction_hex = j_params.at(json_parser::json_keys::sign_command::params::TRANSACTION).get<std::string>();
    std::vector<char> buf(1024);
    auto trans_len = fc::from_hex(transaction_hex, buf.data(), buf.size());
    buf.resize(trans_len);

    unit_list.push_back(buf);
    const auto& keyfile = j_params.at(json_parser::json_keys::sign_command::params::KEYFILE).get<std::string>();

    std::cout <<
              create_response(sign_creatorT::create_signature(get_priv_key_from_file(keyfile.c_str()), unit_list))<< std::endl;
  }


  fc::ecc::private_key get_priv_key_from_file(const char* keyfile)
  {
    auto fin = std::ifstream(keyfile);
    if(!fin.is_open())
      throw std::runtime_error("Error: can't open keyfile");
    if(fin.eof())
      throw std::runtime_error("Error: keyfile is empty");

    std::array<char, 256> buf;
    fin.getline(buf.data(), buf.size());
    if(!fin.good())
    {
      //TODO: add check errno
      throw std::runtime_error("Error: error occured while reading keyfile");
    }
    auto result = graphene::utilities::wif_to_key(std::string(buf.data(), fin.gcount()));
    if(!result)
       throw std::runtime_error("Error: can't get private key from wif string");
    return *result;
  }

  nlohmann::json create_response(const signature_t& signature)
  {
      using out_map = std::map<std::string, nlohmann::json>;
      using out_map_val = out_map::value_type;
      out_map result;
      result.insert(out_map_val(json_parser::json_keys::RESULT,to_hex(signature.begin(),signature.size())));
      return result;
  }

};

}

#endif //KEYCHAINAPP_KEYCHAIN_HPP
