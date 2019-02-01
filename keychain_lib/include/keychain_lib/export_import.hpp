//
// Created by roman on 31/1/19.
//

#pragma once

#include <fc_light/reflect/reflect.hpp>
#include <fc_light/variant.hpp>

#include <boost/filesystem.hpp>
#include <fc_light/filesystem.hpp>

#include "keyfile_parser.hpp"

namespace bfs = boost::filesystem;

namespace keychain_app
{

class secure_dlg_mod_base;

struct export_format
{
  std::string keychain_version;
  keyfile_format::encrypted_data key_data;
};

using key_replace_pair = std::pair<keyfile_format::keyfile_t, keyfile_format::keyfile_t>;
using import_dialog_f = std::function<bool(key_replace_pair)>;

void export_keys(const bfs::path& export_file, const secure_dlg_mod_base* sec_mod);
void import_keys(const bfs::path& import_file, const secure_dlg_mod_base* sec_mod, import_dialog_f&& dialog);

}

FC_LIGHT_REFLECT(keychain_app::export_format, (keychain_version)(key_data))


