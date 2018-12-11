//
// Created by roman on 4/20/18.
//

#include <fstream>
#include <iostream>
#include <array>

#include <fc_light/io/json.hpp>

#include "keychain_commands.hpp"
#include "secmod_protocol.hpp"
#include "keychain.hpp"

namespace keychain_app {

using swap_cmd_t = secmod_commands::secmod_command<secmod_commands::blockchain_secmod_te::ethereum_swap>::type;

bool swap_action(std::string data, swap_cmd_t::swap_t &swap_info) {
  if (data.size() < 8)
    return false;
  
  auto func = data.substr(0, 8);
  
  if (func == SWAP_F1) {
    if (data.length() != 8 + 64 + 64)
      return false;
    
    auto hash = data.substr(8, 64);
    auto address = data.substr(8 + 64, 64);
    swap_info.action = swap_cmd_t::action_te::create_swap;
    swap_info.hash = hash;
    swap_info.address = address;
  } else if (func == SWAP_F2) {
    if (data.length() != 8 + 64)
      return false;
    
    auto address = data.substr(8, 64);
    swap_info.action = swap_cmd_t::action_te::refund;
    swap_info.address = address;
  } else if (func == SWAP_F3) {
    if (data.length() != 8 + 64 + 64)
      return false;
    
    auto secret = data.substr(8, 64);
    auto address = data.substr(8 + 64, 64);
    swap_info.action = swap_cmd_t::action_te::withdraw;
    swap_info.address = address;
    swap_info.secret = secret;
  } else
    return false;
  
  return true;
}

std::string create_secmod_cmd(std::vector<unsigned char> raw, blockchain_te blockchain, std::string from, int unlock_time, std::string keyname)
{
  std::string json;
  auto log = logger_singletone::instance();
  secmod_commands::secmod_command_common cmd;
  cmd.json = true;
  cmd.keyname = keyname;


  switch (blockchain)
  {
    case (keychain_app::blockchain_te::bitcoin):
    {
      cmd.blockchain = secmod_commands::blockchain_secmod_te::bitcoin;
      try
      {
        streambuf_derived buf((char*) raw.data(),  (char*)raw.data() + raw.size());
        std::istream is(&buf);
        
        kaitai::kstream ks(&is);
        bitcoin_transaction_t trx_info(&ks);
        using cmd_t = secmod_commands::secmod_command<secmod_commands::blockchain_secmod_te::bitcoin>::type;
        cmd_t data(std::move(from), std::move(trx_info));
        cmd.data = fc_light::variant(data);
        BOOST_LOG_SEV(log.lg, info) << "bitcoin transaction parse complete: \n";
      } catch (std::exception &exc) {
        cmd.json = false;
        cmd.data = to_hex(raw.data(), raw.size());
        BOOST_LOG_SEV(log.lg, info) << "bitcoin transaction parse is not complete: \n" + std::string(exc.what());
      }
      
      break;
    }
    case (keychain_app::blockchain_te::ethereum):
    {
      try
      {
        auto tx = dev::eth::TransactionBase(raw, dev::eth::CheckTransaction::none);
        secmod_commands::ethereum_trx_t trx;
        trx.nonce     = tx.nonce().str();
        trx.gasPrice  = tx.gasPrice().str();
        trx.gas       = tx.gas().str();
        trx.chainid   = tx.ChainId();
        trx.to        = tx.to().hex();
        trx.value     = tx.value().str();
        
        auto data = to_hex(tx.data().data(), tx.data().size() );
        
        using swap_cmd_t = secmod_commands::secmod_command<secmod_commands::blockchain_secmod_te::ethereum_swap>::type;
        swap_cmd_t::swap_t swap_info;
        if (swap_action(data, swap_info))
        {
          swap_cmd_t data(std::move(from),std::move(trx), std::move(swap_info));
          cmd.blockchain = secmod_commands::blockchain_secmod_te::ethereum_swap;
          cmd.data = fc_light::variant(data);
          BOOST_LOG_SEV(log.lg, info) << "ethereum transaction swap-on-line specific-fields parse complete:";
        }
        else
        {
          using cmd_t = secmod_commands::secmod_command<secmod_commands::blockchain_secmod_te::ethereum>::type;
          cmd_t data(std::move(from),std::move(trx));
          cmd.blockchain = secmod_commands::blockchain_secmod_te::ethereum;
          cmd.data = fc_light::variant(data);
          BOOST_LOG_SEV(log.lg, info) << "ethereum transaction parse complete:";
        }
      }
      catch (const std::exception& exc)
      {
        cmd.json = false;
        cmd.blockchain = secmod_commands::blockchain_secmod_te::ethereum;
        cmd.data = to_hex(raw.data(), raw.size());
        BOOST_LOG_SEV(log.lg, info) << "ethereum transaction parse is not complete: \n" + std::string(exc.what());
      }
      break;
    }
    default:
    {
      cmd.json = false;
      cmd.blockchain = secmod_commands::blockchain_secmod_te::unknown;
      cmd.data = to_hex(raw.data(), raw.size());
      BOOST_LOG_SEV(log.lg, info) << " transaction parse is not implementated:";
    }
  }
  cmd.unlock_time = unlock_time;
  auto variant = fc_light::variant(cmd);
  BOOST_LOG_SEV(log.lg, info) << "\n" + fc_light::json::to_pretty_string(variant);
  return fc_light::json::to_string(variant);
}

}

using namespace keychain_app;

std::pair<std::string, std::string> keychain_app::read_private_key_file(keychain_base* keychain,
        std::string keyname, std::string text, int unlock_time, const keychain_command_base * cmd)
{
  keyfile_format::keyfile_t keyfile;
  auto curdir = bfs::current_path();
  auto first = bfs::directory_iterator(bfs::path(KEY_DEFAULT_PATH_));
  auto it = std::find_if(first, bfs::directory_iterator(),find_keyfile_by_username(keyname.c_str(), &keyfile));
  if (it == bfs::directory_iterator())
    throw std::runtime_error("Error: keyfile could not found by keyname");

  if(keyfile.keyinfo.encrypted)
  {
    auto encrypted_data = keyfile.keyinfo.priv_key_data.as<keyfile_format::encrypted_data>();
    auto& encryptor = encryptor_singletone::instance();
//TODO: need to try to parse transaction.
// If we can parse transaction we need to use get_passwd_trx function
// else use get_passwd_trx_raw()
// At this moment parsing of transaction is not implemented

    byte_seq_t passwd;
    if (cmd->e_type == command_te::unlock)
        passwd = *(keychain->get_passwd_unlock(keyfile.keyname, unlock_time));
    else
        passwd = *(keychain->get_passwd_trx(text.empty() ? keyfile.keyname: text));
    if (passwd.empty())
      throw std::runtime_error("Error: can't get password");
    return  std::make_pair(encryptor.decrypt_keydata(passwd, encrypted_data), keyfile.keyname);
  }
  else
    return  std::make_pair(keyfile.keyinfo.priv_key_data.as<std::string>(), keyfile.keyname);
}

std::pair<std::string, std::string> keychain_app::read_public_key_file(keychain_base* keychain, std::string keyname)
{
  keyfile_format::keyfile_t keyfile;
  auto curdir = bfs::current_path();
  auto first = bfs::directory_iterator(bfs::path(KEY_DEFAULT_PATH_));
  auto it = std::find_if(first, bfs::directory_iterator(),find_keyfile_by_username(keyname.c_str(), &keyfile));
  if (it == bfs::directory_iterator())
    throw std::runtime_error("Error: keyfile could not found by keyname");

  return  std::make_pair(keyfile.keyinfo.public_key, keyfile.keyname);
}


std::string keychain_app::read_private_key(keychain_base * keychain, std::string keyname, std::string text, int seconds,
                                           const keychain_command_base* cmd)
{
  bool locked = true;
  std::string key_data;

  auto map = keychain->key_map.find(keyname);
  if (map != keychain->key_map.end())
  {
    auto time_stamp = map->second.second.second;
    auto unlock_time = map->second.second.first;
    if ((std::time(nullptr) -  time_stamp) > unlock_time )
      keychain->key_map.erase(map);
    else
      locked = false;
  }

  if (locked)
  {
    key_data = read_private_key_file(keychain, keyname, text, seconds, cmd).first;
    if (seconds) // unlock key
      keychain->key_map[keyname] = std::make_pair(key_data, std::make_pair(seconds, std::time(nullptr) ) );
  }
  else
  {
    if (seconds) // unlock key
    {
      key_data = read_private_key_file(keychain, keyname, text, seconds, cmd).first;
      keychain->key_map[keyname] = std::make_pair(key_data, std::make_pair(seconds, std::time(nullptr) ) );
    }
    else
      key_data = keychain->key_map[keyname].first;
  }

  return key_data;
}

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


namespace bfs = keychain_app::bfs;

void keychain_app::create_keyfile(const char* filename, const fc_light::variant& keyfile_var)
{
  bfs::path filepath(std::string(KEY_DEFAULT_PATH_"/") +std::string(filename));
  if(bfs::exists(filepath))
    throw std::runtime_error("Error: can not create keyfile, file is currently exist");

  auto fout = std::ofstream(filepath.c_str());
  if(!fout.is_open())
    throw std::runtime_error("Error: cannot open keyfile");
  fout << fc_light::json::to_pretty_string(keyfile_var) << std::endl;
}

sha2_256_encoder::sha2_256_encoder()
{
  SHA256_Init( &ctx);
};

sha2_256_encoder::~sha2_256_encoder(){};

void sha2_256_encoder::write(const unsigned char * d, uint32_t dlen )
{
    SHA256_Update( &ctx, d, dlen);
}

std::vector<unsigned char> sha2_256_encoder::result ()
{
  std::vector<unsigned char> out(32);
  SHA256_Final(static_cast<unsigned char *>(out.data()), &ctx);
  return out;
}


sha3_256_encoder::sha3_256_encoder(): ctx(EVP_MD_CTX_create()), m_evp_sha_func (EVP_sha3_256)
{
  EVP_DigestInit_ex(ctx, EVP_sha3_256(), NULL);
};

sha3_256_encoder::~sha3_256_encoder()
{
  EVP_MD_CTX_destroy(ctx);
};

void sha3_256_encoder::write(const unsigned char *d, uint32_t dlen)
{
  EVP_DigestUpdate(ctx, d, dlen);
}

std::vector<unsigned char> sha3_256_encoder::result()
{
  std::vector<unsigned char> out(32);
  unsigned int digest_len;
  int sha_size = EVP_MD_size(m_evp_sha_func());
  FC_LIGHT_ASSERT(out.size() == sha_size, "Invalid sha3_256 hash size");
  EVP_DigestFinal_ex(ctx, static_cast<unsigned char*>(out.data()), &digest_len);
  FC_LIGHT_ASSERT(digest_len == out.size(), "Invalid sha3_256 hash size has been written");
  return out;
}

