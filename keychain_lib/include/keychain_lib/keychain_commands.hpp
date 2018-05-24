//
// Created by roman on 4/20/18.
//

#ifndef KEYCHAINAPP_KEYCHAIN_COMMANDS_HPP
#define KEYCHAINAPP_KEYCHAIN_COMMANDS_HPP

#include <string.h>
#include <iostream>

#include <type_traits>
#include <string>
#include <fc/reflect/reflect.hpp>
#include <fc/variant.hpp>
#include <boost/hana/range.hpp>
#include <boost/filesystem.hpp>

#include <fc/variant.hpp>
#include <fc/io/json.hpp>
#include <fc/crypto/hex.hpp>

//TODO: it is unclear
//      why if we do not include this file we cannot compile reflect parser for keychain_app::keyfile_format::key_file
#include <graphene/chain/protocol/transaction.hpp>

#include <graphene/utilities/key_conversion.hpp>
#include <boost/signals2.hpp>

#include "key_file_parser.hpp"
#include "key_encryptor.hpp"
#include "sign_define.hpp"

namespace keychain_app {

using passwd_f = std::function<std::string()>;

struct keychain_error: std::runtime_error
{
  keychain_error(int id_, const char* errmsg): std::runtime_error(errmsg), id(id_){}
  int id;
};

class keychain_base
{
public:
    using string_list = std::list<std::wstring>;
    keychain_base(std::string&& uid_hash_);
    virtual ~keychain_base();
    virtual void operator()(const fc::variant& command) = 0;
    boost::signals2::signal<std::wstring(const std::string&)> get_passwd_trx_raw;
    boost::signals2::signal<std::wstring(const graphene::chain::transaction&)> get_passwd_trx;
    boost::signals2::signal<std::wstring(const std::string&)> get_passwd;
    boost::signals2::signal<void(const string_list&)> print_mnemonic;
    std::string uid_hash;
};

template <typename char_t>
fc::variant open_keyfile(const char_t* filename)
{
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

void create_keyfile(const char* filename, const fc::variant& keyfile_var);
secp256_private_key get_priv_key_from_str(const std::string& str);
fc::sha256 get_hash(const keychain_app::unit_list_t &list);
void send_response(const signature_t& signature, int id);
void send_response(bool res, int id);
size_t from_hex(const std::string& hex_str, unsigned char* out_data, size_t out_data_len );
std::string to_hex(const uint8_t* data, size_t length);
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
    json_response(const fc::variant& var, int id_):result(var), id(id_){}
    json_response(const char* result_, int id_):result(result_), id(id_){}
    int id;
    fc::variant result;
};

struct json_error
{
  json_error(int id_, const char* str): id(id_), error(str){}
  int id;
  std::string error;
};

namespace hana = boost::hana;
namespace bfs = boost::filesystem;

enum keychain_command_type {
    CMD_UNKNOWN = 0,
    CMD_HELP,
    CMD_LIST,
    CMD_SIGN,
    CMD_CREATE,
    CMD_IMPORT,
    CMD_EXPORT,
    CMD_REMOVE,
    CMD_RESTORE,
    CMD_SEED,
    CMD_PUBLIC_KEY,
    CMD_LAST
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
    const auto &file_path = unit.path().filename();
    
    auto j_keyfile = open_keyfile(file_path.c_str());
    auto keyfile = j_keyfile.as<keyfile_format::keyfile_t>();
    if(m_keyfile)
      *m_keyfile = keyfile;//NOTE: move semantic is not implemented in fc::variant in fact
    return strcmp(m_keyname, keyfile.keyname.c_str()) == 0;
  }
  const char* m_keyname;
  keychain_base* m_pkeychain;
  keyfile_format::keyfile_t* m_keyfile;
};

struct keychain_command_common {
  keychain_command_common (keychain_command_type etype = CMD_UNKNOWN, int id_ = 0)
    : command(etype)
    , id(id_){}
  keychain_command_type command;
  int id;
  fc::variant params;
};

struct keychain_command_base {
    keychain_command_base(keychain_command_type type): e_type(type){}
    virtual ~keychain_command_base(){}
    keychain_command_type e_type;
    virtual void operator()(keychain_base* keychain, const fc::variant& params_variant, int id) const{};
};

template<keychain_command_type cmd>
struct keychain_command: keychain_command_base
{
    keychain_command():keychain_command_base(cmd){}
    virtual ~keychain_command(){}
    using params_t = void;
};

template<>
struct keychain_command<CMD_SIGN> : keychain_command_base
{
    keychain_command():keychain_command_base(CMD_SIGN){}
    virtual ~keychain_command(){}
    struct params
    {
        std::string chainid;
        std::string transaction;
        std::string keyname;
        std::string keyfile;
    };

    using params_t = params;

    virtual void operator()(keychain_base* keychain, const fc::variant& params_variant, int id) const override
    {
      try {
        auto params = params_variant.as<params_t>();
        unit_list_t unit_list;
        fc::ecc::private_key private_key;
        if (!params.chainid.empty())
          unit_list.push_back(fc::sha256(params.chainid));
  
        //NOTE: using vector instead array because move semantic is implemented in the vector
        std::vector<char> buf(1024);
        auto trans_len = fc::from_hex(params.transaction, buf.data(), buf.size());
        buf.resize(trans_len);
  
        keyfile_format::keyfile_t keyfile;
  
        unit_list.push_back(buf);
        if (!params.keyfile.empty())
        {
          fc::variant j_keyfile = open_keyfile(params.keyfile.c_str());
          keyfile = j_keyfile.as<keyfile_format::keyfile_t>();
        }
        else if (!params.keyname.empty())
        {
          auto first = bfs::directory_iterator(bfs::path("./"));
          auto it = std::find_if(first, bfs::directory_iterator(),find_keyfile_by_username(params.keyname.c_str(), &keyfile));
          if (it == bfs::directory_iterator())
            throw std::runtime_error("Error: keyfile could not found by keyname");
        }
        if(keyfile.uid_hash != keychain->uid_hash)
          std::runtime_error("Error: user is not keyfile owner");
  
        std::string key_data;
        if(keyfile.keyinfo.encrypted)
        {
          auto encrypted_data = std::move(keyfile.keyinfo.data.as<keyfile_format::encrypted_data>());
          auto& encryptor = encryptor_singletone::instance();
          //TODO: need to try to parse transaction.
          // If we can parse transaction we need to use get_passwd_trx function
          // else use get_passwd_trx_raw()
          // At this moment parsing of transaction is not implemented
          std::wstring passwd = std::move(*(keychain->get_passwd_trx_raw(params.transaction)));
          key_data = std::move(encryptor.decrypt_keydata(passwd.c_str(), encrypted_data));
        }
        else
        {
          key_data = std::move(keyfile.keyinfo.data.as<std::string>());
        }
        private_key = get_priv_key_from_str(key_data);
        send_response(private_key.sign_compact(get_hash(unit_list)), id);
      }
      catch (const std::exception &exc)
      {
        std::cout << fc::json::to_pretty_string(fc::variant(json_error(id, exc.what()))) << std::endl;
      }
      catch (const fc::exception& exc)
      {
        std::cout << fc::json::to_pretty_string(fc::variant(json_error(0, exc.what()))) << std::endl;
        std::cerr << fc::json::to_pretty_string(fc::variant(json_error(0, exc.to_detail_string().c_str()))) << std::endl;
      }
    }
};

template <>
struct keychain_command<CMD_CREATE>: keychain_command_base
{
    keychain_command():keychain_command_base(CMD_CREATE){}
    ~keychain_command(){}
    struct params
    {
      std::string keyname;
      bool encrypted;
      keyfile_format::cipher_etype algo;
      keyfile_format::keyfile_t::keyinfo_t::curve_etype curve;
    };
    using params_t = params;
    virtual void operator()(keychain_base* keychain, const fc::variant& params_variant, int id) const override
    {
      try
      {
        auto params = params_variant.as<params_t>();
        keyfile_format::keyfile_t keyfile;
        std::string wif_key;
        switch (params.curve)
        {
          case keyfile_format::keyfile_t::keyinfo_t::CURVE_SECP256K1:
          {
            wif_key = std::move(graphene::utilities::key_to_wif(fc::ecc::private_key::generate()));
          }
            break;
          default:
          {
            throw std::runtime_error("Error: unsupported curve format");
          }
        }
        if (params.encrypted)
        {
          auto passwd = std::move(*keychain->get_passwd(std::string("Please, enter password for your new key")));
          auto& encryptor = encryptor_singletone::instance();
          auto enc_data = encryptor.encrypt_keydata(params.algo, passwd, wif_key);
          keyfile.keyinfo.data = fc::variant(enc_data);
          keyfile.keyinfo.encrypted = true;
        }
        else{
          keyfile.keyinfo.data = std::move(wif_key);
          keyfile.keyinfo.encrypted = false;
        }
        keyfile.keyname = params.keyname;
        keyfile.uid_hash = keychain->uid_hash;
        keyfile.filetype = keyfile_format::TYPE_KEY;
        keyfile.keyinfo.format = keyfile_format::keyfile_t::keyinfo_t::FORMAT_ARRAYIO;
        keyfile.keyinfo.curve_type = params.curve;
        std::string filename(keyfile.keyname);
        if(filename.empty())
          throw std::runtime_error("Error: keyname (filename) is empty");
        filename += ".json";
        auto first = bfs::directory_iterator(bfs::path("./"));
        auto it = std::find_if(first, bfs::directory_iterator(),find_keyfile_by_username(keyfile.keyname.c_str()));
        if(it != bfs::directory_iterator())
          throw std::runtime_error("Error: keyfile for this user is already exist");
        create_keyfile(filename.c_str(), fc::variant(keyfile));
        send_response(true, id);
      }
      catch (const std::exception &exc)
      {
        std::cout << fc::json::to_pretty_string(fc::variant(json_error(id, exc.what()))) << std::endl;
      }
      catch (const fc::exception& exc)
      {
        std::cout << fc::json::to_pretty_string(fc::variant(json_error(0, exc.what()))) << std::endl;
        std::cerr << fc::json::to_pretty_string(fc::variant(json_error(0, exc.to_detail_string().c_str()))) << std::endl;
      }
    }
};

template <>
struct keychain_command<CMD_REMOVE>: keychain_command_base
{
  keychain_command():keychain_command_base(CMD_REMOVE){}
  ~keychain_command(){}
  struct params
  {
    std::string keyname;
  };
  using params_t = params;
  virtual void operator()(keychain_base* keychain, const fc::variant& params_variant, int id) const override {
    try {
      auto params = params_variant.as<params_t>();
      keyfile_format::keyfile_t keyfile;
      auto first = bfs::directory_iterator(bfs::path("./"));
      auto it = std::find_if(first, bfs::directory_iterator(),find_keyfile_by_username(params.keyname.c_str(), &keyfile));
      if(it != bfs::directory_iterator())
      {
        if(keyfile.uid_hash != keychain->uid_hash)
          throw std::runtime_error("Error: can't remove keyfile because of it is owned by different user");
        bfs::remove(*it);
      }
      send_response(true, id);
    }
    catch (const std::exception &exc)
    {
      std::cout << fc::json::to_pretty_string(fc::variant(json_error(id, exc.what()))) << std::endl;
    }
    catch (const fc::exception& exc)
    {
      std::cout << fc::json::to_pretty_string(fc::variant(json_error(0, exc.what()))) << std::endl;
      std::cerr << fc::json::to_pretty_string(fc::variant(json_error(0, exc.to_detail_string().c_str()))) << std::endl;
    }
  }
};

constexpr auto cmd_static_list =
    hana::make_range(
        hana::int_c<CMD_UNKNOWN>,
        hana::int_c<CMD_LAST>);

}

FC_REFLECT_ENUM(keychain_app::keychain_command_type,
                (CMD_UNKNOWN)(CMD_HELP)(CMD_LIST)(CMD_SIGN)(CMD_CREATE)(CMD_IMPORT)(CMD_EXPORT)(CMD_REMOVE)(CMD_RESTORE)(CMD_SEED)(CMD_PUBLIC_KEY)(CMD_LAST))

FC_REFLECT(keychain_app::keychain_command<keychain_app::CMD_SIGN>::params_t, (chainid)(transaction)(keyname)(keyfile))
FC_REFLECT(keychain_app::keychain_command<keychain_app::CMD_CREATE>::params_t, (keyname)(encrypted)(algo)(curve))
FC_REFLECT(keychain_app::keychain_command<keychain_app::CMD_REMOVE>::params_t, (keyname))
FC_REFLECT(keychain_app::keychain_command_common, (command)(id)(params))
FC_REFLECT(keychain_app::json_response, (id)(result))
FC_REFLECT(keychain_app::json_error, (id)(error))

#endif //KEYCHAINAPP_KEYCHAIN_COMMANDS_HPP
