//
// Created by roman on 4/20/18.
//

#include <fstream>
#include <iostream>
#include <array>

#include <graphene/utilities/key_conversion.hpp>
#include <fc/io/json.hpp>

#include "keychain_commands.hpp"

std::string keychain_app::to_hex(const uint8_t* data, size_t length)
{
  std::string r;
  const char* to_hex_str="0123456789abcdef";
  for(size_t i = 0; i < length; ++i)
    (r += to_hex_str[data[i]>>4]) += to_hex_str[data[i] &0x0f];
  return r;
}

fc::sha256 keychain_app::get_hash(const keychain_app::unit_list_t &list)
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

keychain_app::secp256_private_key keychain_app::get_priv_key_from_str(const std::string& str)
{
  auto result = graphene::utilities::wif_to_key(str);
  if(!result)
    throw std::runtime_error("Error: can't get private key from wif string");
  return *result;
}

fc::variant keychain_app::open_keyfile(const char* filename){
  std::ifstream fin = std::ifstream(filename);
  if(!fin.is_open())
    throw std::runtime_error("Error: cannot open keyfile");
  std::array<char, 1024> read_buf;
  memset(read_buf.data(), 0x00, read_buf.size());
  auto pbuf = read_buf.data();
  auto it = read_buf.begin();
  size_t read_count = 0;
  while(!fin.eof()&&fin.good())
  {
    fin.getline(pbuf, std::distance(it, read_buf.end()));
    pbuf += fin.gcount() - 1;
    it += fin.gcount() - 1;
    read_count += fin.gcount() - 1;
  }
  if(!fin.good()&&read_count==0)
    throw std::runtime_error("Error: cannot read keyfile");
  return fc::json::from_string(std::string(read_buf.begin(), read_buf.end()), fc::json::strict_parser);
}

void keychain_app::send_response(const signature_t& signature)
{
  json_response response(to_hex(signature.begin(),signature.size()).c_str());
  fc::variant res(response);
  std::cout << fc::json::to_pretty_string(res) << std::endl;
}

