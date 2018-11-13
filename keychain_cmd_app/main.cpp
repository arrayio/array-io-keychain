//
// Created by roman on 4/6/18.
//

#include "cmd_parser.hpp"
#include <keychain_lib/logger.hpp>

using namespace keychain_app;



int main(int argc, char* argv[])
{
  boost::shared_ptr< logging::core > core = logging::core::get();
  init();
  logging::add_common_attributes();


  BOOST_LOG_SEV(lg, trace) << "A trace severity message";
  BOOST_LOG_SEV(lg, debug) << "A debug severity message";
  BOOST_LOG_SEV(lg, info) << "An informational severity message";

  core->flush();

  cmd_parser cmd_parser_;
  return cmd_parser_.run(argc, argv);
}

