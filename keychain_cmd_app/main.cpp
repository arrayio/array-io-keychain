//
// Created by roman on 4/6/18.
//

#include "cmd_parser.hpp"

using namespace keychain_app;

int main(int argc, char* argv[])
{
  cmd_parser cmd_parser_;
  return cmd_parser_.run(argc, argv);
}

