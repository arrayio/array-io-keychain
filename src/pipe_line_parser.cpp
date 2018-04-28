//
// Created by roman on 4/6/18.
//

#include "pipe_line_parser.hpp"
#include "keychain.hpp"

#include <stdio.h>
#include <iostream>
#include <stdexcept>
#include <fc/io/json.hpp>

using keychain_t = keychain_app::keychain;

using namespace nlohmann;
using namespace keychain_app;

pipeline_parser::pipeline_parser(keychain_invoke_f&& keychain_f)
  : m_keychain_func(keychain_f)
{
}

int pipeline_parser::run()
{
  buf_type read_buf(4096, 0x00);
  buf_type::value_type *p_read_begin = read_buf.data();
  buf_iterator it_read_end = read_buf.begin();
  size_t bytes_remaining = read_buf.size();
  while (!feof(stdin)){
    size_t bytes_read = fread(p_read_begin, sizeof(buf_type::value_type), 1, stdin);
    if(ferror(stdin))
    {
      std::cerr << "Error: " << strerror(errno) << std::endl;
      return -1;
    }
    it_read_end += bytes_read;
    p_read_begin += bytes_read;
    bytes_remaining -= bytes_read;
    do
    {
      try
      {
        auto buf_range = сut_json_obj(read_buf.begin(), it_read_end);
        if( std::distance(buf_range.first, buf_range.second) > 0)
        {
          m_keychain_func(fc::json::from_string(std::string(buf_range.first, buf_range.second)));

          auto it = std::copy(buf_range.second, it_read_end, read_buf.begin());
          std::for_each(it, it_read_end, [](buf_type::value_type &val) {
              val = 0x00;
          });
          p_read_begin = read_buf.data() + std::distance(read_buf.begin(), it);
          bytes_remaining = std::distance(it, read_buf.end());
          it_read_end = it;
          continue;//try to parse remaining data
        }
        else if(bytes_remaining < 256)
          read_buf.resize(256, 0x00);
        break;//goto fread()
      }
      catch(std::exception& exc)
      {
        std::cout << fc::json::to_pretty_string(fc::variant(json_error(exc.what()))) << std::endl;
        std::cerr << "Error: " << exc.what() << std::endl;
        return -1;
      }
      catch(fc::exception& exc)
      {
        std::cerr << "Error: fc = " << exc.to_detail_string() << std::endl;
        return -1;
      }
    } while (true);
  }
  return 0;
}

pipeline_parser::iter_range pipeline_parser::сut_json_obj(pipeline_parser::buf_iterator parse_begin, pipeline_parser::buf_iterator parse_end)
{
  size_t brace_count = 0;
  auto start_obj = parse_end;
  auto it = parse_begin;
  bool found = false;
  for (; it != parse_end && found == false; ++it)
  {
    switch (*it)
    {
      case json_parser::LBRACE:
        if(brace_count == 0)
          start_obj = it;
        ++brace_count;
        break;
      case json_parser::RBRACE:
        if ( brace_count == 0 )//NOTE: we can inter to this case only, if RBRACE will detect before LBRACE
          throw std::runtime_error("Parse error: common error while counting figure braces");
        --brace_count;
        if (brace_count == 0)
          found = true;
        break;
      case json_parser::SPACE:
      case json_parser::LF:
      case json_parser::CR:
      case json_parser::TAB:
      case json_parser::NP:
      case json_parser::VTAB:
        break;
      default:
        if ( brace_count == 0 )//NOTE: symbols are not into figure braces
          throw std::runtime_error("Parse error: common error while parsing command - unexpected symbols");
    }
  }
  if(found)
  {
    return pipeline_parser::iter_range(start_obj, it);
  }
  else
  {
    return pipeline_parser::iter_range(parse_end, parse_end);
  }
}
