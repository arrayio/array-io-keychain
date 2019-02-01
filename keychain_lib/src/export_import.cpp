//
// Created by roman on 31/1/19
//

#include "secmod_protocol.hpp"
#include "secmod_parser_cmd.hpp"
#include "keyfile_singleton.hpp"
#include "export_import.hpp"
#include "key_encryptor.hpp"
#include "keychain.hpp"

using namespace keychain_app;

namespace secmod_commands = keychain_app::secmod_commands;

fc_light::variant create_secmod_cmd()
{
  secmod_commands::secmod_command cmd;
  cmd.etype = secmod_commands::events_te::export_keys;
  return fc_light::variant(cmd);
}

void keychain_app::export_keys(const bfs::path& export_file, const secure_dlg_mod_base* sec_mod)
{
  auto& keyfiles = keyfile_singleton::instance();
  fc_light::variants export_list;
  std::transform(keyfiles.begin(), keyfiles.end(), std::back_inserter(export_list), [](const auto& val){
    return fc_light::variant(val);
  });
  auto jstr = fc_light::json::to_pretty_string(export_list);
  auto& enc = encryptor_singleton::instance();
  
  auto result = sec_mod->exec_cmd(fc_light::json::to_string(create_secmod_cmd()));
  secmod_commands::secmod_result_parser_f parser;
  byte_seq_t password;
  switch (parser(result))
  {
    case secmod_commands::response_te::password:
      password = std::move(parser.params<secmod_commands::response_te::password>());
      break;
    case secmod_commands::response_te::canceled:
      FC_LIGHT_THROW_EXCEPTION(fc_light::operation_canceled, "");
    default:
      break;
  }
  export_format export_data;
  export_data.key_data = enc.encrypt_data(keyfile_format::cipher_etype::aes256, password, (const unsigned char*)jstr.data(), jstr.size());
  
  auto fout = std::ofstream(export_file.c_str());
  if (!fout.is_open())
    FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "Cannot open keyfile for export (${filename})", ("filename", export_file.c_str()));
  fout << fc_light::json::to_pretty_string(export_data) << std::endl;
}

void keychain_app::import_keys(const bfs::path& import_file, const secure_dlg_mod_base* sec_mod, import_dialog_f&& dialog)
{
  auto import_data = open_keyfile(import_file.c_str());
  auto import_keylist = import_data.as<fc_light::variants>();
  auto& keyfiles = keyfile_singleton::instance();
  const auto& keyfile_ind = keyfiles.prim_index();
  std::for_each(import_keylist.begin(), import_keylist.end(), [&](const auto& val){
    auto keyfile_data_import = val.template as<keyfile_format::keyfile_t>();
    auto it = keyfile_ind.find(keyfile_data_import.keyinfo.public_key);
    if (it != keyfile_ind.end())
    {
      if( dialog(key_replace_pair(*it, keyfile_data_import) ))
        keyfiles.update(std::move(keyfile_data_import));
      else
        return;
    }
    keyfiles.insert(std::move(keyfile_data_import));
  });
}
