//
// Created by roman on 4/6/18.
//

#include "keychain.hpp"
#include <fc/crypto/sha256.hpp>
#include <fc/crypto/elliptic.hpp>

#include <iostream>
#include <errno.h>

using namespace keychain_app;

keychain::keychain(const char* default_key_dir)
  :m_default_path(default_key_dir)
{
  if(!bfs::exists(m_default_path))
  {
    auto res = bfs::create_directory(m_default_path);
    if(res == false)
      throw std::runtime_error("Error: can not create default key directory");
  }
  bfs::current_path(m_default_path);
}

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

secp256_private_key keychain::get_priv_key_from_str(const std::string& str)
{
  auto result = graphene::utilities::wif_to_key(str);
  if(!result)
    throw std::runtime_error("Error: can't get private key from wif string");
  return *result;
}

void keychain::sign_command(const nlohmann::json& j_params)
{
  unit_list_t unit_list;
  fc::ecc::private_key private_key;
  auto chainid_it = j_params.find(json_parser::json_keys::sign_command::params::CHAINID);
  if (chainid_it != j_params.end())
  {
    unit_list.push_back(fc::sha256(chainid_it->get<std::string>()));
  }
  const auto& transaction_hex = j_params.at(json_parser::json_keys::sign_command::params::TRANSACTION).get<std::string>();
  std::vector<char> buf(1024);
  auto trans_len = fc::from_hex(transaction_hex, buf.data(), buf.size());
  buf.resize(trans_len);

  auto check_keyfile_prop = [](const nlohmann::json& j_keyfile)
  {
    if(json_parser::json_file_keys::values::KEY != j_keyfile.at(json_parser::json_file_keys::TYPE).get<std::string>())
      throw std::runtime_error("Error: invalid type of data");
    if(json_parser::json_file_keys::values::SECP256 != j_keyfile.at(json_parser::json_file_keys::CURVE_TYPE).get<std::string>())
      throw std::runtime_error("Error: unsupported curve type");
    if(json_parser::json_file_keys::values::WIF_FORMAT != j_keyfile.at(json_parser::json_file_keys::KEY_DATA_FORMAT).get<std::string>())
      throw std::runtime_error("Error: unsupported key data format");
    if(false != j_keyfile.at(json_parser::json_file_keys::ENCRYPTED).get<bool>())
      throw std::runtime_error("Error: unsupported encrypted keys");
  };

  unit_list.push_back(buf);
  auto keyfile_it = j_params.find(json_parser::json_keys::sign_command::params::KEYFILE);
  if (keyfile_it != j_params.end())
  {
    const auto& keyfile = j_params.at(json_parser::json_keys::sign_command::params::KEYFILE).get<std::string>();
    auto j_keyfile = std::move(open_keyfile(keyfile.c_str()));
    check_keyfile_prop(j_keyfile);
    private_key = get_priv_key_from_str(j_keyfile.at(json_parser::json_file_keys::KEY_DATA).get<std::string>());
  }
  else
  {
    auto keyname_it = j_params.find(json_parser::json_keys::sign_command::params::KEYNAME);
    if (keyname_it == j_params.end())
      throw std::runtime_error("Error: neither keyfile nor keyname parameter was not found");
    nlohmann::json j_keyfile;
    std::string keyname = keyname_it->get<std::string>();
    auto it = std::find_if(bfs::directory_iterator(bfs::path("./")), bfs::directory_iterator(), [this, &j_keyfile, keyname](bfs::directory_entry& unit)->bool
    {
      if (!bfs::is_regular_file(unit.status()))
        return false;
      const auto& file_path = unit.path().filename();
      j_keyfile = std::move(open_keyfile(file_path.c_str()));
      return keyname == j_keyfile.at(json_parser::json_file_keys::USERNAME).get<std::string>();
    });
    if (it == bfs::directory_iterator())
      throw std::runtime_error("Error: keyfile could not found by username");
    check_keyfile_prop(j_keyfile);
    private_key = get_priv_key_from_str(j_keyfile.at(json_parser::json_file_keys::KEY_DATA).get<std::string>());
  }

  std::cout <<
            create_response(private_key.sign_compact(get_hash(unit_list)))<< std::endl;
}

nlohmann::json keychain::open_keyfile(const char* filename)
{
  std::ifstream fin = std::ifstream(filename);
  if(!fin.is_open())
    throw std::runtime_error("Error: cannot open keyfile");
  nlohmann::json j_keyfile;
  fin >> j_keyfile;
  return j_keyfile;
}