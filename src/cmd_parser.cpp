//
// Created by roman on 4/6/18.
//

#include <iostream>

#include "cmd_parser.hpp"
#include "pipe_line_parser.hpp"
#include "keychain_wrapper.hpp"

using namespace keychain_app;

cmd_parser::cmd_parser()
{
  //TODO: define program options
}

int cmd_parser::run(int argc, const char* const argv [])
{
  //TODO: add boost program options call that will parse command line arguments

  keychain_invoke_f f = std::bind(&keychain_dummy_wrapper::exec, std::placeholders::_1);
  pipeline_parser pipe_line_parser_(std::move(f));
  return pipe_line_parser_.run();
}


