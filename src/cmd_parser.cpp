//
// Created by roman on 4/6/18.
//

#include "cmd_parser.hpp"
#include "pipe_line_parser.hpp"
#include <iostream>

using namespace keychain_app;

cmd_parser::cmd_parser()
{
  //TODO: define program options
}

int cmd_parser::run(int argc, const char* const argv [])
{
  //TODO: add boost program options call that will parse command line arguments
  pipeline_parser pipe_line_parser_;
  return pipe_line_parser_.run();
}


