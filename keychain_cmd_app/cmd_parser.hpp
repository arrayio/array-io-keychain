//
// Created by roman on 4/6/18.
//

#ifndef KEYCHAINAPP_CMD_PARSER_HPP
#define KEYCHAINAPP_CMD_PARSER_HPP

#include <boost/program_options.hpp>

#if (defined(linux) || defined(__linux) || defined(__linux__) || defined(__GNU__) || defined(__GLIBC__)) && !defined(_CRAYC)
    #define LINUX
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    #define WIN
#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
    #define APPLE
#endif


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
