//
// Created by roman on 4/6/18.
//

#include <iostream>

#include <keychain_lib/pipeline_parser.hpp>
#include <keychain_lib/keychain_wrapper.hpp>
#include <keychain_lib/secure_module_singletone.hpp>

#include "cmd_parser.hpp"
#include "secure_module_dummy.hpp"

using namespace keychain_app;

cmd_parser::cmd_parser()
{
  //TODO: define program options
}

int cmd_parser::run(int argc, const char* const argv [])
{
  //TODO: add boost program options call that will parse command line arguments
  
  const secure_dlg_mod_base* sec_mod = secure_module<sec_mod_dummy>::instance();

  keychain_invoke_f f = std::bind(&keychain_wrapper, sec_mod, std::placeholders::_1);
  pipeline_parser pipe_line_parser_(std::move(f), fileno(stdin));
  return pipe_line_parser_.run();
}


