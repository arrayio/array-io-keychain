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

using swap_trx_t = secmod_commands::trx_view<secmod_commands::blockchain_secmod_te::ethereum_swap>::type;

bool swap_action(std::string data, swap_trx_t::swap_t &swap_info) {
  if (data.size() < 8)
    return false;
  
  auto func = data.substr(0, 8);
  
  if (func == SWAP_F1) {
    if (data.length() != 8 + 64 + 64)
      return false;
    
    auto hash = data.substr(8, 40);
    auto address = data.substr(8 + 64+24, 40);
    swap_info.action = swap_trx_t::action_te::create_swap;
    swap_info.hash = hash;
    swap_info.address = address;
  } else if (func == SWAP_F2) {
    if (data.length() != 8 + 64)
      return false;
    
    auto address = data.substr(8+24, 40);
    swap_info.action = swap_trx_t::action_te::refund;
    swap_info.address = address;
  } else if (func == SWAP_F3) {
    if (data.length() != 8 + 64 + 64)
      return false;
    
    auto secret = data.substr(8, 64);
    auto address = data.substr(8 + 64+24, 40);
    swap_info.action = swap_trx_t::action_te::withdraw;
    swap_info.address = address;
    swap_info.secret = secret;
  } else
    return false;
  
  return true;
}

fc_light::variant create_secmod_signhex_cmd(std::vector<unsigned char> raw, blockchain_te blockchain, std::string from, int unlock_time, std::string keyname)
{
  std::string json;
  auto& log = logger_singleton::instance();
  secmod_commands::secmod_command cmd;
  using params_t = secmod_commands::secmod_event<secmod_commands::events_te::sign_hex>::params_t;
  params_t params;
  params.is_parsed = true;
  params.keyname = keyname;
  
  switch (blockchain)
  {
    case (keychain_app::blockchain_te::bitcoin):
    {
      params.blockchain = secmod_commands::blockchain_secmod_te::bitcoin;
      try
      {
        streambuf_derived buf((char*) raw.data(),  (char*)raw.data() + raw.size());
        std::istream is(&buf);
        
        kaitai::kstream ks(&is);
        bitcoin_transaction_t trx_info(&ks);
        using trx_t = secmod_commands::trx_view<secmod_commands::blockchain_secmod_te::bitcoin>::type;
        trx_t data(std::move(from), std::move(trx_info));
        params.trx_view = fc_light::variant(data);
        BOOST_LOG_SEV(log.lg, info) << "bitcoin transaction parse complete: \n";
      } catch (std::exception &exc) {
        params.is_parsed = false;
        params.trx_view = to_hex(raw.data(), raw.size());
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
        trx.data      = to_hex(tx.data().data(), tx.data().size());
        
        using swap_trx_t = secmod_commands::trx_view<secmod_commands::blockchain_secmod_te::ethereum_swap>::type;
        swap_trx_t::swap_t swap_info;
        if (swap_action(trx.data, swap_info))
        {
          swap_trx_t data(std::move(from),std::move(trx), std::move(swap_info));
          params.blockchain = secmod_commands::blockchain_secmod_te::ethereum_swap;
          params.trx_view = fc_light::variant(data);
          BOOST_LOG_SEV(log.lg, info) << "ethereum transaction swap-on-line specific-fields parse complete:";
        }
        else
        {
          using trx_t = secmod_commands::trx_view<secmod_commands::blockchain_secmod_te::ethereum>::type;
          trx_t data(std::move(from),std::move(trx));
          params.blockchain = secmod_commands::blockchain_secmod_te::ethereum;
          params.trx_view = fc_light::variant(data);
          BOOST_LOG_SEV(log.lg, info) << "ethereum transaction parse complete:";
        }
      }
      catch (const std::exception& exc)
      {
        params.is_parsed = false;
        params.blockchain = secmod_commands::blockchain_secmod_te::ethereum;
        params.trx_view = to_hex(raw.data(), raw.size());
        BOOST_LOG_SEV(log.lg, info) << "ethereum transaction parse is not complete: \n" + std::string(exc.what());
      }
      break;
    }
    default:
    {
      params.is_parsed = false;
      params.blockchain = secmod_commands::blockchain_secmod_te::unknown;
      params.trx_view = to_hex(raw.data(), raw.size());
      BOOST_LOG_SEV(log.lg, info) << " transaction parse is not implementated:";
    }
  }
  params.unlock_time = unlock_time;
  cmd.etype = secmod_commands::events_te::sign_hex;
  cmd.params = params;
  return fc_light::variant(cmd);
}

fc_light::variant create_secmod_signhash_cmd(const std::string& raw, std::string from, std::string keyname)
{
  secmod_commands::secmod_command cmd;
  using params_t = secmod_commands::secmod_event<secmod_commands::events_te::sign_hash>::params_t;
  params_t params;
  params.hash = raw;
  params.from = std::move(from);
  params.keyname = std::move(keyname);
  
  cmd.etype = secmod_commands::events_te::sign_hash;
  cmd.params = params;
  
  return fc_light::variant(cmd);
}

void keychain_base::lock_all_priv_keys()
{
  key_map.clear();
}

dev::Secret keychain_base::get_private_key(const dev::Public& public_key, int unlock_time, keychain_base::create_secmod_cmd_f&& create_cmd_func)
{
  dev::Secret result;
  do
  {
    if(unlock_time == 0)
    {
      auto it = key_map.find(public_key);
      if(it == key_map.end())
        break;
      auto now = std::chrono::system_clock::now();
      int unlock_duration = std::chrono::duration_cast<std::chrono::seconds>(now - it->unlock_time_point).count();
      if (unlock_duration > it->unlock_duration)
      {
        key_map.erase(it);
        break;
      }
      return it->secret;
    }
  } while (false);
  
  
  if (!result)
  {
    auto& keyfiles = keyfile_singleton::instance();
    auto keyfile = keyfiles[public_key];
    if(keyfile.keyinfo.encrypted)
    {
      byte_seq_t passwd;
      if(create_cmd_func)
        passwd = std::move(*(get_passwd_trx(create_cmd_func(keyfile.keyname))));
      else
        passwd = std::move(*(get_passwd_unlock(keyfile.keyname, unlock_time)));
      if (passwd.empty())
        FC_LIGHT_THROW_EXCEPTION(fc_light::password_input_exception, "");
      auto encrypted_data = keyfile.keyinfo.priv_key_data.as<keyfile_format::encrypted_data>();
      auto& encryptor = encryptor_singleton::instance();
      result = encryptor.decrypt_private_key(passwd, encrypted_data);
      if(unlock_time > 0)
        key_map.insert(private_key_item(result, unlock_time));
    }
  }
  return result;
}

}

using namespace keychain_app;

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
  FC_LIGHT_THROW_EXCEPTION(fc_light::bad_cast_exception, "Invalid hex character");
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

sha2_256_encoder::sha2_256_encoder()
{
  SHA256_Init( &ctx);
};

sha2_256_encoder::~sha2_256_encoder(){};

void sha2_256_encoder::write(const char * d, uint32_t dlen )
{
    SHA256_Update( &ctx, d, dlen);
}

sha2_256_encoder::result_t sha2_256_encoder::result ()
{
  result_t out;
  SHA256_Final(static_cast<unsigned char *>(out.data()), &ctx);
  return out;
}

/*
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
*/
