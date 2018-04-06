//
// Created by roman on 4/6/18.
//

#include "pipe_line_parser.hpp"
#include "keychain.hpp"

#include <stdio.h>
#include <iostream>
#include <stdexcept>
#include <json/json.hpp>

using namespace nlohmann;
using namespace keychain_app;

pipe_line_parser::pipe_line_parser()
  : m_buf(4096, 0x00)
  , m_current(m_buf.data())
  , m_obj_begin (m_buf.begin())
  , m_brace_count(0)
{
}

int pipe_line_parser::run()
{
  while (!feof(stdin)){
    size_t bytes_remaining = m_buf.size();
    size_t bytes_read = fread(m_current, sizeof(buf_type::value_type), 1, stdin);
    if(ferror(stdin))
    {
      //TODO: add check errno
      std::cerr << "error occured while reading stin" << std::endl;
      return -1;
    }
    bytes_remaining -= bytes_read;
    do
    {
      auto buf_range = сut_json_obj();
      if( std::distance(buf_range.first, buf_range.second) > 0)
      {
        keychain<secp256k1_sign_t> keychain_;
        keychain_(json::parse(buf_range.first, buf_range.second));
        auto current_it = std::copy(buf_range.second, m_buf.end(), m_buf.begin());
        m_current = m_buf.data() + std::distance(m_buf.begin(), current_it);
        continue;
      }
      else if(bytes_remaining < 256)
      {
        m_buf.resize(256, 0x00);
      }
      m_current += bytes_read;
      break;
    } while (true);
  }
  return 0;
}



