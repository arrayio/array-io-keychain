//
// Created by roman on 10/12/18.
//

#include <fc_light/variant.hpp>

#include "version_info.hpp"
#include <boost/algorithm/string/replace.hpp>
#include <openssl/opensslv.h>

#include <fc_light/time.hpp>
#include <regex>
#include <fc_light/exception/exception.hpp>

using namespace keychain_app::version_info;

about_info keychain_app::version_info::about()
{
  about_info info;
  
  info.version = version();
  info.git_revision_sha = git_revision_sha;
  info.git_revision_age = fc_light::get_approximate_relative_time_string( fc_light::time_point_sec( git_revision_unix_timestamp ) );
  info.compile_date = "compiled on " __DATE__ " at " __TIME__;
  info.boost_version = boost::replace_all_copy(std::string(BOOST_LIB_VERSION), "_", ".");
  info.openssl_version = OPENSSL_VERSION_TEXT;
  std::string bitness = boost::lexical_cast<std::string>(8 * sizeof(int*)) + "-bit";
#if defined(__APPLE__)
  std::string os = "osx";
#elif defined(__linux__)
  std::string os = "linux";
#elif defined(_WIN32)
  std::string os = "win32";
#else
  std::string os = "other";
#endif
  info.build = os + " " + bitness;
  return info;
}

std::string keychain_app::version_info::version()
{
  static const std::regex VERSION_REGEXP("^\\d+\\.\\d+(\\-\\d)*");
  std::string description_string( git_revision_description );
  auto it = std::sregex_iterator(description_string.begin(), description_string.end(), VERSION_REGEXP);
  if(it == std::sregex_iterator())
    FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "Invalid version string has been returned by Git");
  auto version_string = it->str();
  auto replace_pos = version_string.find('-');
  if (replace_pos != std::string::npos)
    version_string[replace_pos] = '.';
  return version_string;
}

std::string keychain_app::version_info::short_version()
{
  static const std::regex VERSION_REGEXP("^\\d+\\.\\d+");
  auto ver_str = version_info::version();
  auto it = std::sregex_iterator(ver_str.begin(), ver_str.end(), VERSION_REGEXP);
  return it->str();
}


