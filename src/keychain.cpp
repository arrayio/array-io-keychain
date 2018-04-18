//
// Created by roman on 4/6/18.
//

#include "keychain.hpp"
#include <fc/crypto/sha256.hpp>
#include <fc/crypto/elliptic.hpp>

#include <iostream>
#include <errno.h>

using namespace keychain_app;

std::string keychain_app::to_hex(const uint8_t* data, size_t length)
{
  std::string r;
  const char* to_hex_str="0123456789abcdef";
  for(size_t i = 0; i < length; ++i)
    (r += to_hex_str[data[i]>>4]) += to_hex_str[data[i] &0x0f];
  return r;
}

nlohmann::json keychain::create_response(const signature_t& signature)
{
  using out_map = std::map<std::string, nlohmann::json>;
  using out_map_val = out_map::value_type;
  out_map result;
  result.insert(out_map_val(json_parser::json_keys::RESULT,to_hex(signature.begin(),signature.size())));
  return result;
}

void keychain::operator()(const nlohmann::json& message)
{
  if (!message.is_object())
    throw std::runtime_error("Error: json object excpected");
  const auto& command = message.at(json_parser::json_keys::COMMAND).get<std::string>();
  if (command == json_parser::json_keys::sign_command::CMD_NAME)
    sign_command(message.at(json_parser::json_keys::PARAMS));
}

fc::sha256 keychain::get_hash(const keychain_app::unit_list_t &list)
{
  class unit_visitor: public boost::static_visitor<>
  {
  public:
      unit_visitor(fc::sha256::encoder& enc): m_enc(enc){}

      void operator()(const fc::sha256& val)
      {
        m_enc << val;
      }

      void operator()(const std::vector<char>& val)
      {
        return m_enc.write(static_cast<const char*>(val.data()), val.size());
      }
      fc::sha256::encoder& m_enc;
  };

  fc::sha256::encoder enc;
  unit_visitor var_visitor(enc);
  std::for_each(list.begin(), list.end(),[&var_visitor](const unit_t& val){
      boost::apply_visitor(var_visitor, val );
  });
  return enc.result();
}

secp256_private_key keychain::get_priv_key_from_file(const char* keyfile)
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
    std::string errstr = "Error: ";
    errstr += strerror(errno);
    throw std::runtime_error(errstr);
  }
  auto result = graphene::utilities::wif_to_key(std::string(buf.data(), fin.gcount()));
  if(!result)
    throw std::runtime_error("Error: can't get private key from wif string");
  return *result;
}

void keychain::sign_command(const nlohmann::json& j_params)
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

  auto private_key = get_priv_key_from_file(keyfile.c_str());

  std::cout <<
            create_response(private_key.sign_compact(get_hash(unit_list)))<< std::endl;
}