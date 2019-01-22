//
// Created by roman on 4/6/18.
//

#include "cmd_parser.hpp"
#include <keychain_lib/keychain_logger.hpp>
#include <keychain_lib/version_info.hpp>


using namespace keychain_app;
namespace attrs = boost::log::attributes;


int main(int argc, char* argv[])
{
  auto& log = logger_singleton::instance();

  BOOST_LOG_SEV(log.lg, info) << "KeyChain application started, version = " << keychain_app::version_info::version();

#ifdef LINUX
  BOOST_LOG_SEV(log.lg, info) << "OS: Linux";
#elif defined(_MSC_VER)
  BOOST_LOG_SEV(log.lg, info) << "OS: Win";
#else
  BOOST_LOG_SEV(log.lg, info) << "OS: unknown";
#endif

  cmd_parser cmd_parser_;
  return cmd_parser_.run(argc, argv);
}

