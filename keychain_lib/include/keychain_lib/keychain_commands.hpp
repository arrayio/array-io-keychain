// Created by roman on 4/20/18.
//

#ifndef KEYCHAINAPP_KEYCHAIN_COMMANDS_HPP
#define KEYCHAINAPP_KEYCHAIN_COMMANDS_HPP

#include <string.h>
#include <iostream>

#include <type_traits>
#include <string>

#include <boost/hana/range.hpp>
#include <boost/filesystem.hpp>

#include <eth-crypto/core/FixedHash.h>
#include <eth-crypto/crypto/Common.h>

#include <fc_light/variant.hpp>
#include <fc_light/io/json.hpp>
#include <fc_light/io/raw.hpp>
#include <fc_light/reflect/reflect.hpp>
#include <fc_light/reflect/variant.hpp>
#include <fc_light/exception/exception.hpp>
#include <fc_light/crypto/hex.hpp>

#include <boost/signals2.hpp>

#include "eth_types_conversion.hpp"
#include "keyfile_parser.hpp"
#include "key_encryptor.hpp"
#include "sign_define.hpp"
#include <secp256k1_ext.hpp>

#include <openssl/sha.h>
#include <openssl/evp.h>
#include <eth-crypto/core/sha3_wrap.h>
#include "keychain_logger.hpp"
#include <ctime>
#include <eth-crypto/core/TransactionBase.h>
#include <eth-crypto/core/Common.h>

#include <kaitai/kaitaistream.h>
#include "bitcoin_transaction.hpp"
#include <regex>
#include <fc_light/crypto/ripemd160.hpp>
#include <fc_light/crypto/sha256.hpp>
#include <fc_light/crypto/base58.hpp>
#include "private_keymap.hpp"
#include "keyfile_singleton.hpp"
#include "secmod_protocol.hpp"
#include "sql_singleton.hpp"
//#include "secmod_parser_cmd.hpp"

#include "version_info.hpp"
#include <arpa/inet.h>


#ifdef __linux__
#  define KEY_DEFAULT_PATH  "/var/keychain"
#  define LOG_DEFAULT_PATH  "/var/keychain/logs"
#else

#if defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
        #define KEY_DEFAULT_PATH  "Library/Application Support/keychain"
        #define LOG_DEFAULT_PATH  "Library/Application Support/keychain/logs"
#else

#ifdef _WIN32
#  define KEY_DEFAULT_PATH  "./"
#  define LOG_DEFAULT_PATH  "logs"
#endif //_WIN32

#endif //APPLE
#endif //LINUX

#define KEY_DEFAULT_PATH_ KEY_DEFAULT_PATH "/key_data"
#define SIGN_LOGS_DEFAULT_PATH_ KEY_DEFAULT_PATH "/signlogs_data"

#define SWAP_F1 "a543bae7"    // "createSwap(bytes20,address)"
#define SWAP_F2 "fa89401a"      // "refund(address)"
#define SWAP_F3 "1b258d50"      // "withdraw(bytes32,address)"

namespace keychain_app {

struct keychain_command_base;

enum struct sign_te {
  unknown=0,
  VRS_canonical,
  RSV_noncanonical
};

fc_light::variant create_secmod_signhex_cmd(const std::vector<unsigned char>& raw, blockchain_te blockchain, std::string from, int unlock_time, const std::string& keyname, bool no_password);
fc_light::variant create_secmod_signhash_cmd(const std::string& raw, std::string from, const std::string& keyname, bool no_password);
fc_light::variant create_secmod_unlock_cmd(const std::string& keyname, int unlock_time, bool no_password);
std::string parse_trx(std::string&);

class streambuf_derived : public std::basic_streambuf<char>
{
public:
    streambuf_derived(char * beg, char * end)
    {
        this->setg(beg, beg, end);
    }
};


class sha2_256_encoder
{
public:
    using result_t = dev::FixedHash<32>;
    sha2_256_encoder();
    ~sha2_256_encoder();
    void write(const char * d, uint32_t dlen );
    result_t result();
private:
    SHA256_CTX ctx;
};
/*
class sha3_256_encoder
{
public:
    sha3_256_encoder();
    ~sha3_256_encoder();
    void write(const unsigned  char *d, uint32_t dlen);
    std::vector<unsigned char> result();
private:
    EVP_MD_CTX* ctx;
    std::function<const EVP_MD* (void)> m_evp_sha_func;
};
*/
template<typename encoder_t>
typename encoder_t::result_t get_hash( const unit_list_t &list, encoder_t encoder )
{
  class unit_visitor
  {
  public:
      unit_visitor(encoder_t* enc_): m_enc(enc_){}

      void operator()(const std::vector<unsigned char>& val)
      {
        m_enc->write(reinterpret_cast<const char*>(val.data()), val.size());
      }
      encoder_t * m_enc;
  };

  unit_visitor var_visitor(&encoder);


  std::for_each(list.begin(), list.end(), [&var_visitor](const unit_t &val)
  {  var_visitor(val);  });

  return encoder.result();
}

class keychain_base
{
public:
  using string_list = std::list<std::wstring>;
  using create_secmod_cmd_f = std::function<std::string(const std::string& keyname, bool no_password)>;
  virtual std::string operator()(const fc_light::variant& command) = 0;
  boost::signals2::signal<std::string(const std::string&)> run_secmod_cmd;
  boost::signals2::signal<dev::Public(void)> select_key;
  boost::signals2::signal<bool(void)> entropy;

  dev::Secret get_private_key(const dev::Public& public_key, int unlock_time, create_secmod_cmd_f&& f, std::string& cmd);
  void lock_all_priv_keys();
protected:
  keychain_base();
  virtual ~keychain_base();
private:
  private_key_map_t key_map;
};

template <typename char_t>
fc_light::variant open_keyfile(const char_t* filename)
{
  std::ifstream fin = std::ifstream(filename);
  if(!fin.is_open())
    FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception,
                             "Cannot open keyfile, file = ${keyfile}", ("keyfile", filename));
  std::array<char, 1024> read_buf;
  memset(read_buf.data(), 0x00, read_buf.size());
  auto pbuf = read_buf.data();
  auto it = read_buf.begin();
  size_t read_count = 0;
  while(true)
  {
    fin.getline(pbuf, std::distance(it, read_buf.end()));
        if (fin.eof() || !fin.good())
            break;
    pbuf += fin.gcount() - 1;
    it += fin.gcount() - 1;
    read_count += fin.gcount() - 1;
  }
  if(!fin.good()&&read_count==0)
    FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception,
                             "Cannot read keyfile, file = ${keyfile}", ("keyfile", filename));
  return fc_light::json::from_string(std::string(read_buf.begin(), read_buf.end()), fc_light::json::strict_parser);
}

size_t from_hex(const std::string& hex_str, unsigned char* out_data, size_t out_data_len );
std::string to_hex(const uint8_t* data, size_t length);

struct json_response
{
    json_response(){}
    json_response(const fc_light::variant& var, int id_): id(id_), result(var){}
    json_response(const char* result_, int id_): id(id_), result(result_){}
    json_response(const fc_light::variants& var, int id_): id(id_), result(var){}
    int id;
    fc_light::variant result;
};


struct json_error
{
  json_error(int id_, fc_light::exception_code_te err_code, const std::string& msg_ = "",  const fc_light::variant& trace_ = fc_light::variant())
    : id(id_), error(err_code, msg_, trace_){}

  int id;
  struct error_t
  {
    error_t(fc_light::exception_code_te code_, const std::string& message_, const fc_light::variant& trace_)
      : code(static_cast<int>(code_)), name(code_), message(message_), trace(trace_){}
    error_t(): code(0) {}
    int code;
    fc_light::exception_code_te name;
    std::string message;
    fc_light::variant trace;
  } error;
};

namespace hana = boost::hana;
namespace bfs = boost::filesystem;

enum struct command_te {
  null = 0,
  about,
  version,
  sign_trx,
  sign_hash,
 //   create,
    select_key,
  import_cmd,
  export_cmd,
  restore,
  seed,
  lock,
  unlock,
  list,
  public_key,
  set_unlock_time,
  last
};

struct keychain_command_common {
  keychain_command_common (command_te etype = command_te::null, int id_ = 0)
    : command(etype)
    , id(id_){}
  command_te command;
  int id;
  fc_light::variant params;
};

struct keychain_command_base {
    keychain_command_base(command_te type): e_type(type){}
    virtual ~keychain_command_base(){}
    command_te e_type;
    virtual std::string operator()(keychain_base* keychain, const fc_light::variant& params_variant, int id) const = 0;
};

template<command_te cmd>
struct keychain_command: keychain_command_base
{
    keychain_command():keychain_command_base(static_cast<keychain_app::command_te>(cmd)){}
    virtual ~keychain_command(){}
    virtual std::string operator()(keychain_base* keychain, const fc_light::variant& params_variant, int id) const override
    {
      FC_LIGHT_THROW_EXCEPTION( fc_light::command_not_implemented_exception, "" );
    }
    using params_t = void;
};


template <>
struct keychain_command<command_te::about>: keychain_command_base
{
  keychain_command() : keychain_command_base(command_te::list) {}
  virtual ~keychain_command() {}

  using params_t = void;

  virtual std::string operator()(keychain_base *keychain, const fc_light::variant &params_variant, int id) const override
  {
    json_response response(fc_light::variant(version_info::about()), id);
    return fc_light::json::to_string(fc_light::variant(response));
  }
};

template <>
struct keychain_command<command_te::version>: keychain_command_base
{
  keychain_command() : keychain_command_base(command_te::list) {}
  virtual ~keychain_command() {}

  using params_t = void;

  virtual std::string operator()(keychain_base *keychain, const fc_light::variant &params_variant, int id) const override
  {
    json_response response(fc_light::variant(version_info::version()), id);
    return fc_light::json::to_string(fc_light::variant(response));
  }
};

template<>
struct keychain_command<command_te::select_key>: keychain_command_base
{
  keychain_command() : keychain_command_base(command_te::select_key) {}
  virtual ~keychain_command() {}
  using params_t = void;
  virtual std::string operator()(keychain_base* keychain, const fc_light::variant& params_variant, int id) const override
  {
      auto& keyfiles = keyfile_singleton::instance();
      auto it = keyfiles.begin();
      if ( it==keyfiles.end() )
      {
          auto res = keychain->entropy();
      }

      auto public_key = *keychain->select_key();
    if(!public_key)
      FC_LIGHT_THROW_EXCEPTION(fc_light::public_key_not_selected, "");
    json_response response(to_hex(public_key.data(), public_key.size).c_str(), id);
    return fc_light::json::to_string(fc_light::variant(response));
  }
};

template<>
struct keychain_command<command_te::sign_trx> : keychain_command_base
{
  keychain_command():keychain_command_base(command_te::sign_trx) {}
  virtual ~keychain_command(){}
  struct params
  {
    params():unlock_time(0){};
    std::string transaction;
    blockchain_te blockchain_type;
    dev::Public public_key;
    int unlock_time;
  };
  using params_t = params;

  virtual std::string operator()(keychain_base* keychain, const fc_light::variant& params_variant, int id) const override
  {
    params_t params;
    try
    {
      params = params_variant.as<params_t>();
    }
    FC_LIGHT_CAPTURE_TYPECHANGE_AND_RETHROW (fc_light::invalid_arg_exception, error, "cannot parse command params")

    unit_list_t unit_list;
    dev::Signature signature;
    std::vector<unsigned char> raw(params.transaction.length());
    fc_light::variant json;
    dev::Secret private_key;
    auto& keyfiles = keyfile_singleton::instance();
   /* auto &logs = keyfiles.get_logs(params.public_key);
    int n = std::distance(logs.begin(), logs.end());
    for (int i = 0 ; i < n ; i++)
    {
        auto &a  = logs[i];
        auto b = a.blockchain_type;
    }*/
    //NOTE: using vector instead array because move semantic is implemented in the vector
    auto trans_len = keychain_app::from_hex(params.transaction, raw.data(), raw.size());
    raw.resize(trans_len);

    if (!params.public_key)
      FC_LIGHT_THROW_EXCEPTION(fc_light::invalid_arg_exception, "public_key is not specified");

    auto evaluate_from = [&params]()->std::string {
      switch (params.blockchain_type)
      {
        case blockchain_te::ethereum:
          return dev::toAddress(params.public_key).hex();
        case blockchain_te::bitcoin:
        {
          std::vector<char> pub_bin_key;
          pub_bin_key.reserve(64);
          auto pub_len = std::transform(params.public_key.begin(), params.public_key.end(), std::back_inserter(pub_bin_key), [](auto val){
            return static_cast<char>(val);
          });
          pub_bin_key.insert(pub_bin_key.begin(), 4);
          auto sha256 = fc_light::sha256::hash( (const char*)pub_bin_key.data(), pub_bin_key.size() );
          auto ripemd160 = fc_light::ripemd160::hash( sha256 );

          std::vector<char> keyhash(ripemd160.data(), ripemd160.data()+ripemd160.data_size());
          keyhash.insert(keyhash.begin(), 0);

          sha256 = fc_light::sha256::hash( keyhash.data(), keyhash.size() );
          auto checksum = fc_light::sha256::hash( sha256.data(), sha256.data_size() );

          std::vector<char> addr (checksum.data(), checksum.data()+4 );
          addr.insert(addr.begin(), keyhash.begin(), keyhash.end());

          return fc_light::to_base58(addr.data(), addr.size());
        }
        default:
          return std::string();
      }
    };

    std::string secmod_signhex_cmd;
    private_key = keychain->get_private_key(params.public_key, params.unlock_time, [&evaluate_from, &raw, &params](const std::string& keyname, bool no_password)
    {
      return fc_light::json::to_string(
        create_secmod_signhex_cmd(raw, params.blockchain_type, evaluate_from(), params.unlock_time, keyname, no_password));
    }, secmod_signhex_cmd);

    auto reply = [&keyfiles, &params, &id, &secmod_signhex_cmd](auto& message, const dev::bytes& transaction){
        keyfiles.add_log_record(params.public_key,
                                keyfile_format::log_record(transaction, fc_light::time_point::now(), params.blockchain_type ));
        json_response response(fc_light::variant(message), id);
        fc_light::variant res(response);
        return fc_light::json::to_string(res);
    };

    switch (params.blockchain_type)
    {
      case blockchain_te::bitshares:
      {
        unit_list.push_back(raw);
  
        std::array<unsigned char, 65> signature_;
        std::copy(signature.begin(), signature.end(), signature_.begin());

        sign_canonical(signature_, get_hash(unit_list, sha2_256_encoder()).data(),(unsigned char *) private_key.data() );
        break;
      }
      case blockchain_te::array:
      {
        unit_list.push_back(raw);

        signature = dev::sign(private_key,dev::FixedHash<32>(((byte const*) get_hash(unit_list, dev::openssl::sha3_256_encoder()).data()),
                                   dev::FixedHash<32>::ConstructFromPointerType::ConstructFromPointer));
        break;
      }
      case blockchain_te::ethereum:
      {
        auto hash = dev::ethash::sha3_ethash(raw);
        signature = dev::sign(private_key,hash);
        break;
      }
      case blockchain_te::bitcoin:
      {
          streambuf_derived buf((char*) raw.data(),  (char*)raw.data() + raw.size());
          std::istream is(&buf);
          kaitai::kstream ks(&is);
          bitcoin_transaction_t trx_info(&ks);
          std::vector<dev::Signature> signatures;
          std::vector<dev::Signature>::iterator it;

          auto sign = [&private_key](std::vector<unsigned char>& raw)->dev::Signature{
            unit_list_t unit_list;
            unit_list.push_back(raw);
            auto hash = get_hash(unit_list, sha2_256_encoder());
            unit_list.clear();
            unit_list.push_back(hash.asBytes());
            auto hash2 = get_hash(unit_list, sha2_256_encoder());
            return  dev::sign(private_key,dev::FixedHash<32>(((byte const*) hash2.data()),
                                                                 dev::FixedHash<32>::ConstructFromPointerType::ConstructFromPointer));
        };

        // construct transaction header
        std::stringstream ss;
        std::string trx_header;
        ss << std::setfill('0') << std::hex
           << std::setw(8) << ntohl (trx_info.version) << std::setw(2) << ((int) trx_info.num_vins);
        trx_header = ss.str();

        // construct transaction footer
        std::string trx_footer;
        ss.str("");
        ss << std::setw(2) << ((int) trx_info.num_vouts);
        trx_footer = ss.str();
        for (auto& a : trx_info.vouts)
        {
            ss.str("");
            ss << std::setw(8) << ntohl((int) a.amount) << std::setw(8) << ntohl(a.amount >> 32)
            << std::setw(2) << ((int) a.script_len);

            trx_footer += ss.str();
            trx_footer += a.script_pub_key;
        }
        ss.str("");
        ss << std::setw(8) << ntohl(trx_info.locktime);
        trx_footer += ss.str();

        // construct Signing Message Template for each input and signing it.
        for (int loop =0; loop < trx_info.vins.size(); loop++)
        {
            std::string trx = trx_header;
            for (int input =0; input < trx_info.vins.size(); input++)
            {
                trx += trx_info.vins[input].txid;
                ss.str("");
                ss << std::setw(8) << ntohl(trx_info.vins[input].output_id);
                trx += ss.str();
                if (input == loop) // include scriptSig only signing input
                {
                    ss.str("");
                    ss << std::setw(2) << ((int) trx_info.vins[input].script_len);
                    trx += ss.str();
                    trx += trx_info.vins[input].script_sig;
                }
                trx += trx_info.vins[input].end_of_vin;
            }

            trx += trx_footer;
            ss.str("");
            uint32_t sig_hash_code = 1;
            ss << std::setw(8) << ntohl(sig_hash_code);
            trx += ss.str();

            std::vector<unsigned char> raw(trx.length());
            auto raw_len = keychain_app::from_hex(trx, raw.data(), raw.size());
            raw.resize(raw_len);
            signatures.push_back(sign(raw));
        }

        // construct Signed Transaction
        std::string trx = trx_header;
        it = signatures.begin();
        auto iter = [&it, &signatures]() { assert(it < signatures.end()); return *it++;};
        for (auto& a : trx_info.vins)
        {
            auto sig_b = iter();
            auto sig = sig_b.hex();
            std::string r =sig.substr(0, 64), s = sig.substr(64, 64);

            trx += a.txid;
            ss.str("");
            ss << std::setw(8) << ntohl(a.output_id);
            trx += ss.str();
            ss.str("");
            uint8_t script_len = 0x6a, pushdata_sig = 0x47, header=0x30, sig_length=0x44, integer=2, r_length=0x20,
                    s_length=0x20, sig_hash_code=1, pushdata_pubkey=0x21;
            if (sig_b[0] & 0x80)
            {
                script_len++;
                pushdata_sig++;
                sig_length++;
                r_length++;
                r.insert(0, "00");
            }
            if (sig_b[32] & 0x80)
            {
                script_len++;
                pushdata_sig++;
                sig_length++;
                s_length++;
                s.insert(0, "00");
            }
            // script_len + signature DER-encoded + pub_key
            ss  << std::setw(2) << ((int) script_len)
                << std::setw(2) << ((int) pushdata_sig)
                << std::setw(2) << ((int) header)
                << std::setw(2) << ((int) sig_length)
                << std::setw(2) << ((int) integer)
                << std::setw(2) << ((int) r_length)
                << r
                << std::setw(2) << ((int) integer)
                << std::setw(2) << ((int) s_length)
                << s
                << std::setw(2) << ((int) sig_hash_code)
                << std::setw(2) << ((int) pushdata_pubkey)
                << "03"+ dev::toPublic(private_key).hex().substr(0,64);
            trx += ss.str();
            trx += a.end_of_vin;
        }
        trx += trx_footer;

        return reply(trx, raw);
      }
      default:
        FC_LIGHT_THROW_EXCEPTION(fc_light::invalid_arg_exception,
                                 "Unknown blockchain_type, blockchain = ${type}", ("type", params.blockchain_type));
    }

    if (!signature)
      FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "Resulting signature is null");

    return reply(signature, raw);
  }
};


template<>
struct keychain_command<command_te::sign_hash> : keychain_command_base
{
  keychain_command():keychain_command_base(command_te::sign_hash){}
  virtual ~keychain_command(){}
  struct params
  {
    std::string hash;
    sign_te sign_type;
    dev::Public public_key;
  };

  using params_t = params;

  virtual std::string operator()(keychain_base* keychain, const fc_light::variant& params_variant, int id) const override
  {
    auto& log = logger_singleton::instance();

    params_t params;
    try
    {
      params = params_variant.as<params_t>();
    }
    FC_LIGHT_CAPTURE_TYPECHANGE_AND_RETHROW (fc_light::invalid_arg_exception, error, "cannot parse command params")

    if (!params.public_key)
      FC_LIGHT_THROW_EXCEPTION( fc_light::parse_error_exception, "public_key is not specified" );

    auto& keyfiles = keyfile_singleton::instance();

    auto evaluate_from = [&keychain, &params, &keyfiles]() -> std::string
    {
      return to_hex(params.public_key.data(), params.public_key.size);
    };

      std::string secmod_signhash_cmd;
      //TODO: it is more preferable to use move semantic instead copy for json argument
    auto private_key = keychain->get_private_key(params.public_key, 0, [&evaluate_from, &params](const std::string& keyname, bool no_password)
    {
      return fc_light::json::to_string(
        create_secmod_signhash_cmd(params.hash, evaluate_from(), keyname, no_password));
    }, secmod_signhash_cmd);

    //NOTE: using vector instead array because move semantic is implemented in the vector
    std::vector<unsigned char> hash(params.hash.length());
    auto trans_len = keychain_app::from_hex(params.hash, hash.data(), hash.size());
    hash.resize(trans_len);

    dev::Signature signature;

    switch (params.sign_type)
    {
      case sign_te::VRS_canonical:
      {
        std::array<unsigned char, 65> signature_;
        sign_canonical(signature_, hash.data(),(unsigned char *) private_key.data() );
        signature = dev::Signature(signature_.data(), dev::Signature::ConstructFromPointer);
        break;
      }
      default:
      {
        signature = dev::sign(private_key, dev::FixedHash<32>(((byte const*) hash.data()),
                                   dev::FixedHash<32>::ConstructFromPointerType::ConstructFromPointer));
        break;
      }
    }
  
    dev::bytes hash_vec;
    std::copy(params.hash.begin(), params.hash.end(), std::back_inserter(hash_vec));
    keyfiles.add_log_record(params.public_key,
                            keyfile_format::log_record(hash_vec, fc_light::time_point::now(), blockchain_te::rawhash));
    json_response response(fc_light::variant(signature), id);
    fc_light::variant res(response);
    return fc_light::json::to_string(res);
  }
};

/*TODO: move this function to common code for key manager
template <>
struct keychain_command<command_te::create>: keychain_command_base
{
    keychain_command():keychain_command_base(command_te::create){}
    virtual ~keychain_command(){}
    struct params
    {
      std::string keyname;
      std::string description;
      bool encrypted;
      keyfile_format::cipher_etype cipher;
      keyfile_format::curve_etype curve;
    };
    using params_t = params;
    virtual std::string operator()(keychain_base* keychain, const fc_light::variant& params_variant, int id) const override
    {
      //TODO: need to be depreciated when keymanager will be ready
      params_t params;

      try
      {
        params = params_variant.as<params_t>();
      }

      FC_LIGHT_CAPTURE_TYPECHANGE_AND_RETHROW (fc_light::invalid_arg_exception, error, "cannot parse command params")
      params.cipher = keyfile_format::cipher_etype::aes256;
      params.curve  = keyfile_format::curve_etype::secp256k1;
      params.encrypted = true;

      auto& keyfiles = keyfile_singleton::instance();
      keyfile_format::keyfile_t keyfile;
      dev::Secret priv_key;
      dev::Public pb_hex;
      dev::h256 hash;
      std::string filename, keyname;
      switch (params.curve)
      {
        case keyfile_format::curve_etype::secp256k1:
        {
          auto keys = dev::KeyPair::create();
          pb_hex = keys.pub();
          hash = dev::ethash::sha3_ethash(keys.pub());
          priv_key = keys.secret();
          filename    = hash.hex().substr(0,16);
          keyname       = params.keyname ;
          filename += ".json";
        }
          break;
        default:
        {
          FC_LIGHT_THROW_EXCEPTION(fc_light::invalid_arg_exception,
                                   "Unsupported curve format, curve = ${type}", ("type", params.curve));
        }
      }

      auto secmod_cmd = [](const std::string& keyname){
        secmod_commands::secmod_command cmd;
        using params_t = secmod_commands::secmod_event<secmod_commands::events_te::create_key>::params_t;
        params_t params;
        params.keyname = keyname;
        params.keyname = std::move(keyname);
  
        cmd.etype = secmod_commands::events_te::create_key;
        cmd.params = params;
  
        return fc_light::variant(cmd);
      };
      
      if (params.encrypted)
      {
        auto result = std::move(*(keychain->run_secmod_cmd(fc_light::json::to_string(secmod_cmd(params.keyname)))));
        secmod_commands::secmod_result_parser_f parser;
        byte_seq_t passwd;
        switch (parser(result))
        {
        auto passwd = *keychain->get_passwd_on_create(keyname);
              FC_LIGHT_THROW_EXCEPTION(fc_light::password_input_exception, "");
              secmod_commands::secmod_command cmd;
              cmd.params = fc_light::variant(params);
          default:
            FC_LIGHT_THROW_EXCEPTION(fc_light::password_input_exception, "");
          auto result = std::move(*(keychain->run_secmod_cmd(create_secmod_createkey_cmd(keyfile.keyname))));
          secmod_commands::secmod_result_parser_f parser;
          byte_seq_t password;
          switch (parser(result)) {
              case secmod_commands::response_te::password: {
                  password = std::move(parser.params<secmod_commands::response_te::password>());
                  if (password.empty())
                      FC_LIGHT_THROW_EXCEPTION(fc_light::password_input_exception, "");
                  auto &encryptor = encryptor_singleton::instance();
                  auto enc_data = encryptor.encrypt_private_key(keyfile_format::cipher_etype::aes256, password, priv_key);
                  keyfile.keyinfo.priv_key_data = fc_light::variant(enc_data);
                  keyfile.keyinfo.encrypted = true;
              }
          }

      }
      else{
        keyfile.keyinfo.priv_key_data = fc_light::variant(priv_key);
        keyfile.keyinfo.encrypted = false;
      }

      keyfile.keyinfo.public_key = pb_hex;
      keyfile.keyname = keyname;
      keyfile.description = params.description;
      keyfile.creation_time = fc_light::time_point::now();
      keyfile.keychain_version = version_info::short_version();
      keyfile.filetype = keyfile_format::TYPE_KEY;
      keyfile.keyinfo.curve_type = params.curve;

      if(filename.empty())//TODO: need to fix error output, need to provide params info
        FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "Keyname (filename) is empty");

      keyfiles.insert(std::move(keyfile));
      json_response response(keyname, id);
      return fc_light::json::to_string(fc_light::variant(response));
    }
};
*/

template <>
struct keychain_command<command_te::list>: keychain_command_base {
  keychain_command() : keychain_command_base(command_te::list) {}
  virtual ~keychain_command() {}

  using params_t = void;

  virtual std::string operator()(keychain_base *keychain, const fc_light::variant &params_variant, int id) const override
  {
    FC_LIGHT_THROW_EXCEPTION(fc_light::command_depreciated, "Use \"select_key\" command instead");
  }
};

template<>
struct keychain_command<command_te::public_key>: keychain_command_base
{
  keychain_command(): keychain_command_base(command_te::public_key){}
  virtual ~keychain_command(){}
  struct params
  {
    std::string keyname;
  };
  using params_t = params;
  virtual std::string operator()(keychain_base* keychain, const fc_light::variant& params_variant, int id) const override
  {
    FC_LIGHT_THROW_EXCEPTION(fc_light::command_depreciated, "Use \"select_key\" command instead");
  }
};

template<>
struct keychain_command<command_te::seed>: keychain_command_base
{
    keychain_command(): keychain_command_base(command_te::seed){}
    virtual ~keychain_command(){}
    struct params {std::string entropy;};
    using  params_t = params;
    virtual std::string operator()(keychain_base* keychain, const fc_light::variant& params_variant, int id) const override
    {
        FC_LIGHT_THROW_EXCEPTION(fc_light::command_depreciated, "");
    }
};


template<>
struct keychain_command<command_te::restore>: keychain_command_base
{
    keychain_command(): keychain_command_base(command_te::restore){}
    virtual ~keychain_command(){}
    struct params {std::string seed;};
    using  params_t = params;
    virtual std::string operator()(keychain_base* keychain, const fc_light::variant& params_variant, int id) const override
    {
        FC_LIGHT_THROW_EXCEPTION(fc_light::command_depreciated, "");
    }
};


template<>
struct keychain_command<command_te::lock>: keychain_command_base
{
  keychain_command(): keychain_command_base(command_te::lock){}
  virtual ~keychain_command(){}
  using  params_t = void;
  virtual std::string operator()(keychain_base* keychain, const fc_light::variant& params_variant, int id) const override
  {
    keychain->lock_all_priv_keys();
    json_response response(true, id);
    return fc_light::json::to_string(fc_light::variant(response));
  }
};

template<>
struct keychain_command<command_te::unlock>: keychain_command_base
{
  keychain_command(): keychain_command_base(command_te::unlock){}
  virtual ~keychain_command(){}
  struct params
  {
    params():unlock_time(0){};
    dev::Public public_key;
    int unlock_time;
  };
  using  params_t = params;

  virtual std::string operator()(keychain_base* keychain, const fc_light::variant& params_variant, int id) const override
  {
    params_t params;
    try
    {
      params = params_variant.as<params_t>();
    }
    FC_LIGHT_CAPTURE_TYPECHANGE_AND_RETHROW (fc_light::invalid_arg_exception, error, "cannot parse command params")

    if (params.unlock_time <= 0)
      FC_LIGHT_THROW_EXCEPTION(fc_light::invalid_arg_exception,
        "unlock_time invalid or not specified, unlock_time = ${UNLOCK_TIME}", ("UNLOCK_TIME", params.unlock_time));

    if (!params.public_key)
      FC_LIGHT_THROW_EXCEPTION(fc_light::invalid_arg_exception, "public_key is not specified");

    std::string secmod_unlock_cmd;
    auto private_key = keychain->get_private_key(params.public_key, params.unlock_time, [&params](const std::string& keyname, bool no_password)
    {
      return fc_light::json::to_string(
        create_secmod_unlock_cmd(keyname, params.unlock_time, no_password));
    }, secmod_unlock_cmd);

    json_response response(true, id);
    return fc_light::json::to_string(fc_light::variant(response));
  }
};

template<>
struct keychain_command<command_te::set_unlock_time>: keychain_command_base
{
    keychain_command(): keychain_command_base(command_te::lock){}
    virtual ~keychain_command(){}
    struct params
    {
      int seconds;
    };
    using  params_t = params;
    virtual std::string operator()(keychain_base* keychain, const fc_light::variant& params_variant, int id) const override
    {
        FC_LIGHT_THROW_EXCEPTION(fc_light::command_depreciated, "");
    }
};


constexpr auto cmd_static_list =
    hana::make_range(
        hana::int_c<static_cast<int>(command_te::null)>,
        hana::int_c<static_cast<int>(command_te::last)>);

}

FC_LIGHT_REFLECT_ENUM(
  keychain_app::command_te,
  (null)
  (about)
  (version)
  (sign_trx)
  (sign_hash)
  //        (create)
  (select_key)
  (import_cmd)
  (export_cmd)
  (restore)
  (seed)
  (lock)
  (unlock)
  (list)
  (public_key)
  (set_unlock_time)
  (last)
)

FC_LIGHT_REFLECT(keychain_app::keychain_command<keychain_app::command_te::sign_trx>::params_t, (transaction)(blockchain_type)(public_key)(unlock_time))
FC_LIGHT_REFLECT(keychain_app::keychain_command<keychain_app::command_te::sign_hash>::params_t, (hash)(sign_type)(public_key))
//FC_LIGHT_REFLECT(keychain_app::keychain_command<keychain_app::command_te::create>::params_t, (keyname)(description)(encrypted)(cipher)(curve))
FC_LIGHT_REFLECT(keychain_app::keychain_command<keychain_app::command_te::public_key>::params_t, (keyname))
FC_LIGHT_REFLECT(keychain_app::keychain_command<keychain_app::command_te::set_unlock_time>::params_t, (seconds))
FC_LIGHT_REFLECT(keychain_app::keychain_command<keychain_app::command_te::unlock>::params_t, (public_key)(unlock_time))
FC_LIGHT_REFLECT(keychain_app::keychain_command<keychain_app::command_te::seed>::params_t, (entropy))
FC_LIGHT_REFLECT(keychain_app::keychain_command<keychain_app::command_te::restore>::params_t, (seed))
FC_LIGHT_REFLECT(keychain_app::keychain_command_common, (command)(id)(params))
FC_LIGHT_REFLECT(keychain_app::json_response, (id)(result))
FC_LIGHT_REFLECT(keychain_app::json_error::error_t, (code)(name)(message)(trace))
FC_LIGHT_REFLECT(keychain_app::json_error, (id)(error))
FC_LIGHT_REFLECT_ENUM(keychain_app::sign_te, (unknown)(VRS_canonical)(RSV_noncanonical))

#endif //KEYCHAINAPP_KEYCHAIN_COMMANDS_HPP
