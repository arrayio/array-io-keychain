//
// Created by roman on 4/6/18.
//

#ifndef KEYCHAINAPP_KEYCHAIN_HPP
#define KEYCHAINAPP_KEYCHAIN_HPP

#include <functional>
#include <iostream>

#include <fc/crypto/elliptic.hpp>
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
namespace command_params
{

static const char* CHAINID = "chainid";
static const char* HDPATH = "hdpath";
static const char* TRANSACTION_HASH = "transaction-hash";
static const char* KEYFILE = "keyfile";

}
}
}

template <typename sign_creatorT>
class keychain : public  sign_creatorT
{
public:
  void operator()(const nlohmann::json& message) {
    std::string command;
    std::string chainid;
    std::string transaction_hash;
    std::string keyfile;
    unit_list_t unit_list;
    if (!message.is_object())
      throw std::runtime_error("Error: json object excpected");
    command = message.at(json_parser::json_keys::COMMAND).get<std::string>();
    auto j_params = message.at(json_parser::json_keys::PARAMS);
    auto chainid_it = j_params.find(json_parser::json_keys::command_params::CHAINID);
    if (chainid_it != j_params.end())
    {
      unit_list.push_back(fc::sha256(chainid_it->get<std::string>()));
    }
    transaction_hash = j_params.at(json_parser::json_keys::command_params::TRANSACTION_HASH).get<std::string>();
    unit_list.push_back(fc::sha256(transaction_hash));
    keyfile = j_params.at(json_parser::json_keys::command_params::KEYFILE).get<std::string>();

    std::cout <<
      create_response(sign_creatorT::create_signature(get_priv_key_from_file(keyfile.c_str()), unit_list))<< std::endl;
  }


    fc::ecc::private_key get_priv_key_from_file(const char* keyfile)
    {
        constexpr size_t sha256_size = sizeof(fc::sha256);
        auto file = fopen(keyfile, "r");
        if (file == nullptr)
            throw std::runtime_error("Error: Unable to open keyfile");
        std::array<char, sha256_size*2> buf;
        std::cin.getline(buf.data(), buf.size());
        if(!std::cin.good())
        {
            //TODO: add check errno
            throw std::runtime_error("Error: error occured while reading keyfile");
        }
        if(std::cin.gcount() != sha256_size)
        {
            throw std::runtime_error("Error: invalid key length");
        }
        return fc::ecc::private_key::regenerate(fc::sha256(std::string(buf.begin(), buf.begin()+sha256_size)));
    }

    nlohmann::json create_response(const signature_t& signature)
    {
        using out_map = std::map<std::string, nlohmann::json>;
        using out_map_val = out_map::value_type;
        out_map result;
        result.insert(out_map_val(json_parser::json_keys::RESULT, std::string(signature.begin(), signature.end())));
        return result;
    }

};

}

#endif //KEYCHAINAPP_KEYCHAIN_HPP
