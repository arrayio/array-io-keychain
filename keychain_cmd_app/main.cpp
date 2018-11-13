//
// Created by roman on 4/6/18.
//

#include "cmd_parser.hpp"
#include <keychain_lib/logger.hpp>


using namespace keychain_app;
namespace attrs = boost::log::attributes;


int main(int argc, char* argv[])
{
  auto log = logger_singletone::instance();

  BOOST_LOG_SEV(log.lg, info) << "Start";

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

