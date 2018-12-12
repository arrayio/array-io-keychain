//
// Created by roman on 10/12/18.
//

#include <fc_light/variant.hpp>

#include "version_info.hpp"
#include <boost/algorithm/string/replace.hpp>
#include <openssl/opensslv.h>

#include <fc_light/time.hpp>

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
  std::string version_string( git_revision_description );
  const size_t pos = version_string.find( '/' );
  if( pos != std::string::npos && version_string.size() > pos )
    version_string = version_string.substr( pos + 1 );
  
  return version_string;
}


