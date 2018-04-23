//
// Created by roman on 4/20/18.
//

#ifndef KEYCHAINAPP_KEYCHAIN_COMMANDS_HPP
#define KEYCHAINAPP_KEYCHAIN_COMMANDS_HPP

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

#include "key_file_parser.hpp"
#include "sign_define.hpp"

namespace keychain_app {

fc::variant open_keyfile(const char* filename);
secp256_private_key get_priv_key_from_str(const std::string& str);
fc::sha256 get_hash(const keychain_app::unit_list_t &list);
void send_response(const signature_t& signature);
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


struct keychain_command_common {
    keychain_command_type command;
    fc::variant params;
};

struct keychain_command_base {
    keychain_command_base(keychain_command_type type): e_type(type){}
    virtual ~keychain_command_base(){}
    keychain_command_type e_type;
    virtual void operator()(const fc::variant& params_variant) const{};
};

template<keychain_command_type cmd>
struct keychain_command: keychain_command_base
{
    keychain_command():keychain_command_base(cmd){}
    virtual ~keychain_command(){}
    using params_t = void;
};

template<>
struct keychain_command<CMD_SIGN> : keychain_command_base {
    keychain_command():keychain_command_base(CMD_SIGN){}
    virtual ~keychain_command(){}
    struct params {
        std::string chainid;
        std::string transaction;
        std::string keyname;
        std::string keyfile;
    };

    using params_t = params;

    virtual void operator()(const fc::variant& params_variant) const override {
      auto params = params_variant.as<params_t>();
      unit_list_t unit_list;
      fc::ecc::private_key private_key;
      if (!params.chainid.empty())
        unit_list.push_back(fc::sha256(params.chainid));

      //NOTE: using vector instead array because move semantic is implemented in the vector
      std::vector<char> buf(1024);
      auto trans_len = fc::from_hex(params.transaction, buf.data(), buf.size());
      buf.resize(trans_len);

      unit_list.push_back(buf);
      if (!params.keyfile.empty()) {
        fc::variant j_keyfile = open_keyfile(params.keyfile.c_str());
        auto keyfile = j_keyfile.as<keyfile_format::key_file>();
        private_key = get_priv_key_from_str(keyfile.keyinfo.data);
      } else if (!params.keyname.empty()) {
        keyfile_format::key_file keyfile;
        auto first = bfs::directory_iterator(bfs::path("./"));
        auto it = std::find_if(first, bfs::directory_iterator(),[this, params, &keyfile](bfs::directory_entry &unit) -> bool {
                                   if (!bfs::is_regular_file(unit.status()))
                                     return false;
                                   const auto &file_path = unit.path().filename();
                                   auto j_keyfile = open_keyfile(file_path.c_str());
                                   keyfile = j_keyfile.as<keyfile_format::key_file>();
                                   return params.keyname == keyfile.username;
                               });
        if (it == bfs::directory_iterator())
          throw std::runtime_error("Error: keyfile could not found by username");
        private_key = get_priv_key_from_str(keyfile.keyinfo.data);
      }
      send_response(private_key.sign_compact(get_hash(unit_list)));
    }
};

constexpr auto cmd_static_list =
    hana::make_range(
        hana::int_c<CMD_UNKNOWN>,
        hana::int_c<CMD_LAST>);

}

FC_REFLECT_ENUM(keychain_app::keychain_command_type,
                (CMD_UNKNOWN)(CMD_HELP)(CMD_LIST)(CMD_SIGN)(CMD_CREATE)(CMD_IMPORT)(CMD_EXPORT)(CMD_REMOVE)(CMD_RESTORE)(CMD_SEED)(CMD_PUBLIC_KEY)(CMD_LAST))

FC_REFLECT(typename keychain_app::keychain_command<keychain_app::CMD_SIGN>::params_t, (chainid)(transaction)(keyname)(keyfile))
FC_REFLECT(keychain_app::keychain_command_common, (command)(params))
FC_REFLECT(keychain_app::json_response, (result))

#endif //KEYCHAINAPP_KEYCHAIN_COMMANDS_HPP
