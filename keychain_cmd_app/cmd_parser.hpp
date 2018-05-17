//
// Created by roman on 4/6/18.
//

#ifndef KEYCHAINAPP_CMD_PARSER_HPP
#define KEYCHAINAPP_CMD_PARSER_HPP

#include <boost/program_options.hpp>

namespace po = boost::program_options;

namespace keychain_app
{

class cmd_parser
{
public:
  cmd_parser();
  int run(int argc, const char* const argv[]);

private:
  po::options_description m_options;

  int parse_cmd_line(int argc, const char* const argv[]);
};


}

#endif //KEYCHAINAPP_CMD_PARSER_HPP
