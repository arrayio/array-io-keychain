//
// Created by roman on 4/20/18.
//

#ifndef KEYCHAINAPP_KEYCHAIN_COMMANDS_HPP
#define KEYCHAINAPP_KEYCHAIN_COMMANDS_HPP

#include <string.h>

#include <type_traits>
#include <string>
#include <fc/reflect/reflect.hpp>
#include <fc/variant.hpp>
#include <boost/hana/range.hpp>
#include <boost/filesystem.hpp>


#include <fc/crypto/hex.hpp>

//TODO: it is unclear
//      why if we do not include this file we cannot compile reflect parser for keychain_app::keyfile_format::key_file
#include <graphene/chain/protocol/transaction.hpp>

#include <graphene/utilities/key_conversion.hpp>

#include "key_file_parser.hpp"
#include "key_encryptor.hpp"
#include "sign_define.hpp"

namespace keychain_app {

using passwd_f = std::function<std::string()>;

class keychain_base
{
public:
    keychain_base(passwd_f&& get_password);
    virtual ~keychain_base();
    virtual void operator()(const fc::variant& command) = 0;
    passwd_f get_passwd_functor;
};

fc::variant open_keyfile(const char* filename);
void create_keyfile(const char* filename, const fc::variant& keyfile_var);
secp256_private_key get_priv_key_from_str(const std::string& str);
fc::sha256 get_hash(const keychain_app::unit_list_t &list);
void send_response(const signature_t& signature);
void send_response(bool res);
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
    json_response(const fc::variant& var):result(var){}
    json_response(const char* result_):result(result_){}
    fc::variant result;
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
    return strcmp(m_keyname, keyfile.username.c_str()) == 0;
  }
  const char* m_keyname;
  keyfile_format::keyfile_t* m_keyfile;
};

struct keychain_command_common {
    keychain_command_type command;
    fc::variant params;
};

struct keychain_command_base {
    keychain_command_base(keychain_command_type type): e_type(type){}
    virtual ~keychain_command_base(){}
    keychain_command_type e_type;
    virtual void operator()(keychain_base* keychain, const fc::variant& params_variant) const{};
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

    virtual void operator()(keychain_base* keychain, const fc::variant& params_variant) const override
    {
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
          throw std::runtime_error("Error: keyfile could not found by username");
      }
  
      std::string key_data;
      if(keyfile.keyinfo.encrypted)
      {
        auto encrypted_data = std::move(keyfile.keyinfo.data.as<keyfile_format::encrypted_data>());
        auto& encryptor = encryptor_singletone::instance();
        std::string passwd = std::move(keychain->get_passwd_functor());
        key_data = std::move(encryptor.decrypt_keydata(passwd.c_str(), encrypted_data));
      }
      else
      {
        key_data = std::move(keyfile.keyinfo.data.as<std::string>());
      }
      private_key = get_priv_key_from_str(key_data);
      send_response(private_key.sign_compact(get_hash(unit_list)));
    }
};

template <>
struct keychain_command<CMD_CREATE>: keychain_command_base
{
    keychain_command():keychain_command_base(CMD_CREATE){}
    ~keychain_command(){}
    struct params
    {
      std::string username;
      bool encrypted;
      keyfile_format::cipher_etype algo;
      keyfile_format::keyfile_t::keyinfo_t::curve_etype curve;
    };
    using params_t = params;
    virtual void operator()(keychain_base* keychain, const fc::variant& params_variant) const override
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
        auto passwd = std::move(keychain->get_passwd_functor());
        auto& encryptor = encryptor_singletone::instance();
        auto enc_data = encryptor.encrypt_keydata(params.algo, passwd, wif_key);
        keyfile.keyinfo.data = fc::variant(enc_data);
        keyfile.keyinfo.encrypted = true;
      }
      else{
        keyfile.keyinfo.data = std::move(wif_key);
        keyfile.keyinfo.encrypted = false;
      }
      keyfile.username = params.username;
      keyfile.filetype = keyfile_format::TYPE_KEY;
      keyfile.keyinfo.format = keyfile_format::keyfile_t::keyinfo_t::FORMAT_ARRAYIO;
      keyfile.keyinfo.curve_type = params.curve;
      std::string filename(keyfile.username);
      filename += ".json";
      auto first = bfs::directory_iterator(bfs::path("./"));
      auto it = std::find_if(first, bfs::directory_iterator(),find_keyfile_by_username(keyfile.username.c_str()));
      //TODO: need check not only filename but username field in key file
      if(it != bfs::directory_iterator(bfs::path()))
        throw std::runtime_error("Error: keyfile for this user is already exist");
      create_keyfile(filename.c_str(), fc::variant(keyfile));
      send_response(true);
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
FC_REFLECT(keychain_app::keychain_command<keychain_app::CMD_CREATE>::params_t, (username)(encrypted)(algo)(curve))
FC_REFLECT(keychain_app::keychain_command_common, (command)(params))
FC_REFLECT(keychain_app::json_response, (result))

#endif //KEYCHAINAPP_KEYCHAIN_COMMANDS_HPP
