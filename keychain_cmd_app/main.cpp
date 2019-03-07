//
// Created by roman on 4/6/18.
//

#include "cmd_parser.hpp"
#include <keychain_lib/keychain_logger.hpp>
#include <keychain_lib/version_info.hpp>
#include <iostream>

using namespace keychain_app;
namespace attrs = boost::log::attributes;


int main(int argc, char* argv[])
{
//    std::cout << "Start keychain" << std::endl;
  auto& log = logger_singleton::instance();
//    std::cout << "auto keychain" << std::endl;

  BOOST_LOG_SEV(log.lg, info) << "KeyChain application started, version = " << keychain_app::version_info::version();
//    std::cout << "BOOST_LOG_SEV keychain" << std::endl;

#ifdef LINUX
  BOOST_LOG_SEV(log.lg, info) << "OS: Linux";
#elif defined(_MSC_VER)
  BOOST_LOG_SEV(log.lg, info) << "OS: Win";
#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
  BOOST_LOG_SEV(log.lg, info) << "OS: Mac";
#else
  BOOST_LOG_SEV(log.lg, info) << "OS: unknown";
#endif
//    std::cout << "boost keychain" << std::endl;

  cmd_parser cmd_parser_;
//    std::cout << "cmd_parser_ keychain" << std::endl;

  return cmd_parser_.run(argc, argv);
}

