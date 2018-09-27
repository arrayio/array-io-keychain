//
// Created by roman on 4/20/18.
//

#include <fstream>
#include <iostream>
#include <array>

#include <fc_light/io/json.hpp>

#include "keychain_commands.hpp"
#include <openssl/sha.h>

std::string keychain_app::to_hex(const uint8_t* data, size_t length)
{
  std::string r;
  const char* to_hex_str="0123456789abcdef";
  for(size_t i = 0; i < length; ++i)
    (r += to_hex_str[data[i]>>4]) += to_hex_str[data[i] &0x0f];
  return r;
}

uint8_t from_hex_symbol( char c )
{
  if( c >= '0' && c <= '9' )
    return c - '0';
  if( c >= 'a' && c <= 'f' )
    return c - 'a' + 10;
  if( c >= 'A' && c <= 'F' )
    return c - 'A' + 10;
  throw std::runtime_error("Error: Invalid hex character");
}

size_t keychain_app::from_hex( const std::string& hex_str, unsigned char* out_data, size_t out_data_len )
{
  auto it = hex_str.begin();
  uint8_t* out_pos = out_data;
  uint8_t* out_end = out_pos + out_data_len;
  while( it != hex_str.end() && out_end != out_pos ) {
    *out_pos = from_hex_symbol(*it) << 4;
    ++it;
    if( it != hex_str.end() )  {
      *out_pos |= from_hex_symbol(*it);
      ++it;
    }
    ++out_pos;
  }
  return out_pos - out_data;
}

std::vector<unsigned char> keychain_app::get_hash(const keychain_app::unit_list_t &list)
{
  class unit_visitor: public boost::static_visitor<>
  {
  public:
      unit_visitor() {SHA256_Init( &ctx); }

      void operator()(const std::vector<char>& val)
      {
          SHA256_Update( &ctx, static_cast<const char*>(val.data()), val.size());
      }

      std::vector<unsigned char>  result()
      {
          std::vector<unsigned char>  out(32);
          SHA256_Final(out.data(), &ctx);
          return out;
      }
      SHA256_CTX ctx;
  };


  unit_visitor var_visitor;
  std::for_each(list.begin(), list.end(),[&var_visitor](const unit_t& val){
      boost::apply_visitor(var_visitor, val );
  });
  return var_visitor.result();
}

namespace bfs = keychain_app::bfs;

void keychain_app::create_keyfile(const char* filename, const fc_light::variant& keyfile_var)
{
  bfs::path filepath(filename);
  if(bfs::exists(filepath))
    throw std::runtime_error("Error: can not create keyfile, file is currently exist");
  auto path = bfs::current_path();
  path += bfs::path("/");
  path += filepath;
  auto fout = std::ofstream(filename);
  if(!fout.is_open())
    throw std::runtime_error("Error: cannot open keyfile");
  fout << fc_light::json::to_pretty_string(keyfile_var) << std::endl;
}

using namespace keychain_app;

keychain_base::keychain_base(std::string&& uid_hash_)
  : uid_hash(std::move(uid_hash_))
{}

keychain_base::~keychain_base(){}
