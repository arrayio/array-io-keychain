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

#include <fc_light/variant.hpp>
#include <fc_light/io/json.hpp>
#include <fc_light/reflect/reflect.hpp>
#include <fc_light/reflect/variant.hpp>
#include <fc_light/exception/exception.hpp>
#include <fc_light/crypto/hex.hpp>

#include <fc_light/exception/exception.hpp>

#include <boost/signals2.hpp>

#include "key_file_parser.hpp"
#include "key_encryptor.hpp"
#include "sign_define.hpp"
#include <eth-crypto/core/FixedHash.h>
#include <eth-crypto/crypto/Common.h>
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
#include "secmod_protocol.hpp"

#include "version_info.hpp"

#ifdef __linux__
#  define KEY_DEFAULT_PATH  "/var/keychain"
#  define LOG_DEFAULT_PATH  "/var/keychain/logs"
#else

#if defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
        #define KEY_DEFAULT_PATH  "/var/keychain"
        #define LOG_DEFAULT_PATH  "/var/keychain/logs"
#else

#ifdef _WIN32
#  define KEY_DEFAULT_PATH  "./"
#  define LOG_DEFAULT_PATH  "./logs"
#endif //_WIN32

#endif //APPLE
#endif //LINUX

#define KEY_DEFAULT_PATH_ KEY_DEFAULT_PATH "/key_data"
#define SWAP_F1 "a543bae7"    // "createSwap(bytes20,address)"
#define SWAP_F2 "fa89401a"      // "refund(address)"
#define SWAP_F3 "1b258d50"      // "withdraw(bytes32,address)"

namespace keychain_app {

struct keychain_command_base;

enum struct blockchain_te {
  unknown=0,
  array,
  bitshares,
  ethereum,
  bitcoin
};

enum struct sign_te {
  unknown=0,
  VRS_canonical,
  RSV_noncanonical
};

std::string create_secmod_cmd(std::vector<unsigned char> raw, blockchain_te blockchain, std::string from, int unlock_time, std::string keyname);

using byte_seq_t = std::vector<char>;

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
    sha2_256_encoder();
    ~sha2_256_encoder();
    void write(const unsigned  char * d, uint32_t dlen );
    std::vector<unsigned char> result ();
private:
    SHA256_CTX ctx;
};

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

template<typename encoder_t>
std::vector<unsigned char> get_hash( const unit_list_t &list, encoder_t encoder )
{
  class unit_visitor
  {
  public:
      unit_visitor(encoder_t* enc_): m_enc(enc_){}

      void operator()(const std::vector<unsigned char>& val)
      {
        m_enc->write( static_cast<const unsigned char*>(val.data()), val.size());
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
    keychain_base();
    virtual ~keychain_base();
    virtual std::string operator()(const fc_light::variant& command) = 0;
    boost::signals2::signal<byte_seq_t(const std::string&)> get_passwd_trx;
    boost::signals2::signal<byte_seq_t(const std::string&,int)> get_passwd_unlock;//TODO: need to call in unlock command handler
    boost::signals2::signal<byte_seq_t(void)> get_passwd_on_create;
    boost::signals2::signal<void(const string_list&)> print_mnemonic;

    // {keyname, {private_key {unlock_time, time_stamp}} }
    std::unordered_map<std::string, std::pair<std::string, std::pair<int, std::time_t>>> key_map;
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

void create_keyfile(const char* filename, const fc_light::variant& keyfile_var);
size_t from_hex(const std::string& hex_str, unsigned char* out_data, size_t out_data_len );
std::string to_hex(const uint8_t* data, size_t length);
std::string read_private_key(keychain_base *, std::string , std::string, int, const keychain_command_base*);
std::pair<std::string, std::string> read_public_key_file( keychain_base * , std::string );
std::pair<std::string, std::string> read_private_key_file( keychain_base * , std::string , std::string,
                                                           int unlock_time, const keychain_command_base* );



/*{
  using out_map = std::map<std::string, nlohmann::json>;
  using out_map_val = out_map::value_type;
  out_map result;
  result.insert(out_map_val(json_parser::json_keys::RESULT,to_hex(signature.begin(),signature.size())));
  return result;
}*/

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
  json_error(int id_, fc_light::exception_code err_code, const std::string& msg_ = "",  const fc_light::variant& trace_ = fc_light::variant())
    : id(id_), error(err_code, msg_, trace_){}
    
  int id;
  struct error_t
  {
    error_t(fc_light::exception_code code_, const std::string& message_, const fc_light::variant& trace_)
      : code(static_cast<int>(code_)), name(code_), message(message_), trace(trace_){}
    error_t(): code(0) {}
    int code;
    fc_light::exception_code name;
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
    help,
    list,
    sign_hex,
    sign_hash,
    create,
    import_cmd,
    export_cmd,
    remove,
    restore,
    seed,
    public_key,
    lock,
    unlock,
    set_unlock_time,
    last
};

struct find_keyfile_by_username
{
  find_keyfile_by_username(const char* keyname, keyfile_format::keyfile_t* keyfile = nullptr)
    : m_keyname(keyname)
    , m_keyfile(keyfile)
  {
  }

  bool operator()(bfs::directory_entry &unit)
  {
    if (!bfs::is_regular_file(unit.status()))
      return false;

    auto j_keyfile = open_keyfile(unit.path().c_str());
    auto keyfile = j_keyfile.as<keyfile_format::keyfile_t>();
    if(m_keyfile)
      *m_keyfile = keyfile;//NOTE: move semantic is not implemented in fc_light::variant in fact
    return strcmp(m_keyname, keyfile.keyname.c_str()) == 0;
  }
  const char* m_keyname;
  keyfile_format::keyfile_t* m_keyfile;
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
struct keychain_command<command_te::about>: keychain_command_base {
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
struct keychain_command<command_te::version>: keychain_command_base {
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
struct keychain_command<command_te::sign_hex> : keychain_command_base
{
  keychain_command():keychain_command_base(command_te::sign_hex) {}
  virtual ~keychain_command(){}
  struct params
  {
    params():unlock_time(0){};
    std::string chainid;
    std::string transaction;
    blockchain_te blockchain_type;
    std::string keyname;
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
    dev::Secret private_key;
    std::array<unsigned char, 65> signature = {0};
    std::vector<unsigned char> chain(32);
    std::vector<unsigned char> raw(params.transaction.length());
    std::string json;
    std::string from;
    std::string pub_key = read_public_key_file(keychain, params.keyname).first;
  
    if (!params.chainid.empty())
        auto chain_len = keychain_app::from_hex(params.chainid, chain.data(), chain.size());
  
    //NOTE: using vector instead array because move semantic is implemented in the vector
    auto trans_len = keychain_app::from_hex(params.transaction, raw.data(), raw.size());
    raw.resize(trans_len);
  
    if (params.keyname.empty())
      FC_LIGHT_THROW_EXCEPTION(fc_light::invalid_arg_exception, "Keyname is not specified");
  
    switch (params.blockchain_type)
    {
      case blockchain_te::ethereum:
      {
        from  = dev::toAddress(dev::FixedHash<64>(pub_key)).hex();
        break;
      }
      case blockchain_te::bitcoin:
      {
        std::vector<char> pub_bin_key(64, 0);
        auto pub_len = keychain_app::from_hex(pub_key, (unsigned char *) pub_bin_key.data(), pub_bin_key.size());
        pub_bin_key.insert(pub_bin_key.begin(), 4);
        auto sha256 = fc_light::sha256::hash( pub_bin_key.data(), pub_bin_key.size() );
        auto ripemd160 = fc_light::ripemd160::hash( sha256 );

        std::vector<char> keyhash(ripemd160.data(), ripemd160.data()+ripemd160.data_size());
        keyhash.insert(keyhash.begin(), 0);

        sha256 = fc_light::sha256::hash( keyhash.data(), keyhash.size() );
        auto checksum = fc_light::sha256::hash( sha256.data(), sha256.data_size() );

        std::vector<char> addr (checksum.data(), checksum.data()+4 );
        addr.insert(addr.begin(), keyhash.begin(), keyhash.end());

        from = fc_light::to_base58(addr.data(), addr.size());
        break;
      }
      default:
        from = "";
    }
  
    json = create_secmod_cmd(raw, params.blockchain_type, from, params.unlock_time, params.keyname);
    std::string key_data = read_private_key(keychain, params.keyname, json , params.unlock_time, this);
    int pk_len = keychain_app::from_hex(key_data, (unsigned char*) private_key.data(), 32);
  
    switch (params.blockchain_type)
    {
      case blockchain_te::bitshares:
      {
        if (chain.size())
          unit_list.push_back(chain);
        unit_list.push_back(raw);

        sign_canonical(signature, get_hash(unit_list, sha2_256_encoder()).data(),(unsigned char *) private_key.data() );
        break;
      }
      case blockchain_te::array:
      {
        if (chain.size())
          unit_list.push_back(chain);
        unit_list.push_back(raw);

        signature = dev::sign(private_key,dev::FixedHash<32>(((byte const*) get_hash(unit_list, sha3_256_encoder()).data()),
                                   dev::FixedHash<32>::ConstructFromPointerType::ConstructFromPointer)).asArray();
        break;
      }
      case blockchain_te::ethereum:
      {
        auto hash = dev::ethash::sha3_ethash(raw);
        signature = dev::sign(private_key,hash).asArray();
        break;
      }
      case blockchain_te::bitcoin:
      {
        unit_list.push_back(raw);
        auto hash = get_hash(unit_list, sha2_256_encoder());
        unit_list.clear();
        unit_list.push_back(hash);
        auto hash2 = get_hash(unit_list, sha2_256_encoder());
        signature = dev::sign(private_key,dev::FixedHash<32>(((byte const*) hash2.data()),
                                   dev::FixedHash<32>::ConstructFromPointerType::ConstructFromPointer)).asArray();
        break;
      }
      default:
        FC_LIGHT_THROW_EXCEPTION(fc_light::invalid_arg_exception,
                                 "Unknown blockchain_type, blockchain = ${type}", ("type", params.blockchain_type));
    }
  
    json_response response(to_hex(signature.data(), signature.size()).c_str(), id);
    fc_light::variant res(response);
    return fc_light::json::to_string(res);
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
    std::string keyname;
  };

  using params_t = params;

  virtual std::string operator()(keychain_base* keychain, const fc_light::variant& params_variant, int id) const override
  {
    auto log = logger_singletone::instance();
  
    params_t params;
    try
    {
      params = params_variant.as<params_t>();
    }
    FC_LIGHT_CAPTURE_TYPECHANGE_AND_RETHROW (fc_light::invalid_arg_exception, error, "cannot parse command params")
    
    dev::Secret private_key;

    if (params.keyname.empty())
      FC_LIGHT_THROW_EXCEPTION( fc_light::parse_error_exception, "keyname is not specified" );
    std::string pub_key = read_public_key_file(keychain, params.keyname).first;

    using cmd_t = secmod_commands::secmod_command<secmod_commands::blockchain_secmod_te::rawhash>::type;
    cmd_t data(std::move(pub_key), params.hash);

    secmod_commands::secmod_command_common cmd;
    cmd.json = true;
    cmd.keyname = params.keyname;
    cmd.blockchain = secmod_commands::blockchain_secmod_te::rawhash;
    cmd.unlock_time = 0;
    cmd.data = fc_light::variant(data);

    auto variant = fc_light::variant(cmd);
    auto json = fc_light::json::to_string(variant);
    BOOST_LOG_SEV(log.lg, info) << "sign_hash secmodule command: \n" + fc_light::json::to_pretty_string(variant);

    //TODO: it is more preferable to use move semantic instead copy for json argument
    std::string key_data = read_private_key(keychain, params.keyname, json, 0, this );

    int pk_len = keychain_app::from_hex(key_data, (unsigned char*) private_key.data(), 32);

    //NOTE: using vector instead array because move semantic is implemented in the vector
    std::vector<unsigned char> hash(params.hash.length());
    auto trans_len = keychain_app::from_hex(params.hash, hash.data(), hash.size());
    hash.resize(trans_len);

    std::array<unsigned char, 65> signature = {0};

    switch (params.sign_type)
    {
      case sign_te::VRS_canonical:
      {
        sign_canonical(signature, hash.data(),(unsigned char *) private_key.data() );
        break;
      }
      default:
      {
        signature = dev::sign(private_key, dev::FixedHash<32>(((byte const*) hash.data()),
                                   dev::FixedHash<32>::ConstructFromPointerType::ConstructFromPointer)).asArray();
        break;
      }
    }

    json_response response(to_hex(signature.data(), signature.size()).c_str(), id);
    fc_light::variant res(response);
    return fc_light::json::to_string(res);
  }
};

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
      keyfile_format::keyfile_t::keyinfo_t::curve_etype curve;
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
      
      keyfile_format::keyfile_t keyfile;
      std::string pr_hex, pb_hex;
      dev::h256 hash;
      std::string filename, keyname;
      switch (params.curve)
      {
        case keyfile_format::keyfile_t::keyinfo_t::curve_etype::secp256k1:
        {
          dev::KeyPair keys = dev::KeyPair::create();
          pb_hex = keys.pub().hex();
          hash = dev::ethash::sha3_ethash(keys.pub());
          pr_hex = to_hex(reinterpret_cast<const uint8_t *>(keys.secret().data()), 32);

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

      if (params.encrypted)
      {
        auto passwd = *keychain->get_passwd_on_create();
        if (passwd.empty())
          FC_LIGHT_THROW_EXCEPTION(fc_light::password_input_exception, "");
        auto& encryptor = encryptor_singletone::instance();
        auto enc_data = encryptor.encrypt_keydata(params.cipher, passwd, pr_hex);
        keyfile.keyinfo.priv_key_data = fc_light::variant(enc_data);
        keyfile.keyinfo.encrypted = true;
      }
      else{
        keyfile.keyinfo.priv_key_data = std::move(pr_hex);
        keyfile.keyinfo.encrypted = false;
      }
      
      keyfile.keyinfo.public_key = pb_hex;
      keyfile.keyname = keyname;
      keyfile.description = params.description;
      keyfile.keychain_version = version_info::short_version();
      keyfile.filetype = keyfile_format::TYPE_KEY;
      keyfile.keyinfo.format = keyfile_format::keyfile_t::keyinfo_t::FORMAT_ARRAYIO;
      keyfile.keyinfo.curve_type = params.curve;

      if(filename.empty())//TODO: need to fix error output, need to provide params info
        FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "Keyname (filename) is empty");

      auto first = bfs::directory_iterator(bfs::path(KEY_DEFAULT_PATH_));
      auto it = std::find_if(first, bfs::directory_iterator(),find_keyfile_by_username(keyfile.keyname.c_str()));
      if(it != bfs::directory_iterator())//TODO: need to fix error output, need to provide params info
        FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "Keyfile for this user is already exist");
      create_keyfile(filename.c_str(), fc_light::variant(keyfile));

      json_response response(keyname, id);
      return fc_light::json::to_string(fc_light::variant(response));
    }
};

template <>
struct keychain_command<command_te::list>: keychain_command_base {
  keychain_command() : keychain_command_base(command_te::list) {}
  virtual ~keychain_command() {}
  
  using params_t = void;
  
  virtual std::string operator()(keychain_base *keychain, const fc_light::variant &params_variant, int id) const override
  {
    fc_light::variants keyname_list;
    keyname_list.reserve(128);
    auto first = bfs::directory_iterator(bfs::path(KEY_DEFAULT_PATH_));
    std::for_each(first, bfs::directory_iterator(), [&keyname_list](bfs::directory_entry &unit){
      if (!bfs::is_regular_file(unit.status()))
        return;

      auto j_keyfile = open_keyfile(unit.path().c_str());
      auto keyfile = j_keyfile.as<keyfile_format::keyfile_t>();
      keyname_list.push_back(fc_light::variant(std::move(keyfile.keyname)));
      return;
    });
    json_response response(keyname_list, id);
    return fc_light::json::to_string(fc_light::variant(response));
  }
};

/*
template <>
struct keychain_command<command_te::remove>: keychain_command_base
{
  keychain_command():keychain_command_base(command_te::remove){}
  virtual ~keychain_command(){}
  struct params
  {
    std::string keyname;
  };
  using params_t = params;
  virtual std::string operator()(keychain_base* keychain, const fc_light::variant& params_variant, int id) const override {
    throw std::runtime_error("Command not implementated");

    auto params = params_variant.as<params_t>();
    keyfile_format::keyfile_t keyfile;
    auto first = bfs::directory_iterator(bfs::path(KEY_DEFAULT_PATH_));
    auto it = std::find_if(first, bfs::directory_iterator(),find_keyfile_by_username(params.keyname.c_str(), &keyfile));
    if(it != bfs::directory_iterator())
        bfs::remove(*it);
    keychain->key_map.erase(params.keyname);

    json_response response(true, id);
    return fc_light::json::to_string(fc_light::variant(response));
  }
};
*/

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
    params_t params;
    try
    {
      params = params_variant.as<params_t>();
    }
    FC_LIGHT_CAPTURE_TYPECHANGE_AND_RETHROW (fc_light::invalid_arg_exception, error, "cannot parse command params")
    
    keyfile_format::keyfile_t keyfile;

    if (params.keyname.empty())
      FC_LIGHT_THROW_EXCEPTION(fc_light::invalid_arg_exception, "Keyname is not specified");

    auto curdir = bfs::current_path();
    auto first = bfs::directory_iterator(bfs::path(KEY_DEFAULT_PATH_));
    auto it = std::find_if(first, bfs::directory_iterator(),find_keyfile_by_username(params.keyname.c_str(), &keyfile));
    if (it == bfs::directory_iterator())
      FC_LIGHT_THROW_EXCEPTION(fc_light::privkey_not_found_exception, "Keyfile could not found by keyname");
    
    json_response response(keyfile.keyinfo.public_key.c_str(), id);
    return fc_light::json::to_string(fc_light::variant(response));
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
    keychain->key_map.clear();
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
    std::string keyname;
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
    
    if (!params.keyname.empty())
      read_private_key(keychain, params.keyname, "", params.unlock_time, this);
    else
      FC_LIGHT_THROW_EXCEPTION(fc_light::invalid_arg_exception, "Keyname is not specified");

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
//            auto params = params_variant.as<params_t>();
//            json_response response(true, id);
//            return fc_light::json::to_string(fc_light::variant(response));
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
    (help)
    (list)
    (sign_hex)
    (sign_hash)
    (create)
    (import_cmd)
    (export_cmd)
 //   (remove)
    (restore)
    (seed)
    (public_key)
    (lock)
    (unlock)
    (set_unlock_time)
    (last))

FC_LIGHT_REFLECT(keychain_app::keychain_command<keychain_app::command_te::sign_hex>::params_t, (chainid)(transaction)(blockchain_type)(keyname)(unlock_time))
FC_LIGHT_REFLECT(keychain_app::keychain_command<keychain_app::command_te::sign_hash>::params_t, (hash)(sign_type)(keyname))
FC_LIGHT_REFLECT(keychain_app::keychain_command<keychain_app::command_te::create>::params_t, (keyname)(description)(encrypted)(cipher)(curve))
//FC_LIGHT_REFLECT(keychain_app::keychain_command<keychain_app::command_te::remove>::params_t, (keyname))
FC_LIGHT_REFLECT(keychain_app::keychain_command<keychain_app::command_te::public_key>::params_t, (keyname))
FC_LIGHT_REFLECT(keychain_app::keychain_command<keychain_app::command_te::set_unlock_time>::params_t, (seconds))
FC_LIGHT_REFLECT(keychain_app::keychain_command<keychain_app::command_te::unlock>::params_t, (keyname)(unlock_time))
FC_LIGHT_REFLECT(keychain_app::keychain_command_common, (command)(id)(params))
FC_LIGHT_REFLECT(keychain_app::json_response, (id)(result))
FC_LIGHT_REFLECT(keychain_app::json_error::error_t, (code)(name)(message)(trace))
FC_LIGHT_REFLECT(keychain_app::json_error, (id)(error))
FC_LIGHT_REFLECT_ENUM(keychain_app::blockchain_te, (unknown)(bitshares)(array)(ethereum)(bitcoin))
FC_LIGHT_REFLECT_ENUM(keychain_app::sign_te, (unknown)(VRS_canonical)(RSV_noncanonical))

#endif //KEYCHAINAPP_KEYCHAIN_COMMANDS_HPP
