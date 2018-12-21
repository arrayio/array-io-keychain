//
// Created by roman on 10/12/18.
//

#ifndef KEYCHAINAPP_VERSION_INFO_HPP
#define KEYCHAINAPP_VERSION_INFO_HPP

#include <string>
#include <fc_light/variant.hpp>
#include <fc_light/io/json.hpp>
#include <fc_light/reflect/reflect.hpp>
#include <fc_light/reflect/variant.hpp>

#include "git_revision.hpp"

namespace keychain_app { namespace version_info {

struct about_info
{
  std::string version;
  std::string git_revision_sha;
  std::string git_revision_age;
  std::string compile_date;
  std::string boost_version;
  std::string openssl_version;
  std::string build;
};

about_info about(); //return variant map that can be serialized into string,
std::string version();
std::string short_version();

} }

FC_LIGHT_REFLECT(keychain_app::version_info::about_info, (version)(git_revision_sha)(git_revision_age)(compile_date)(boost_version)(openssl_version)(build))


#endif //KEYCHAINAPP_VERSION_INFO_HPP
