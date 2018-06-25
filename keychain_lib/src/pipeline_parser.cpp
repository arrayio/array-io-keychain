//
// Created by roman on 4/6/18.
//

#include "pipeline_parser.hpp"
#include "keychain.hpp"

#include <stdio.h>
#include <iostream>
#include <stdexcept>
#include <fc/io/json.hpp>

#ifdef _MSC_VER
#include <io.h>
#else
#include <unistd.h>
#endif

using keychain_t = keychain_app::keychain;

using namespace keychain_app;

pipeline_parser::pipeline_parser(keychain_invoke_f&& keychain_f, int pipein_desc, int pipeout_desc)
  : m_keychain_func(keychain_f)
  , m_pipein_desc(pipein_desc)
  , m_pipeout_desc(pipeout_desc)
{
}

int pipeline_parser::run()
{
  buf_type read_buf(4096, 0x00);
  buf_type::value_type *p_read_begin = read_buf.data();
  buf_iterator it_read_end = read_buf.begin();
  size_t bytes_remaining = read_buf.size();
  while (true)
  {
    size_t bytes_read = read(m_pipein_desc, p_read_begin, bytes_remaining);
	if ( bytes_read == 0 )
		break;
    if( bytes_read == -1 )
    {
      std::cerr << "Error: " << strerror(errno) << std::endl;
      return -1;
    }
	it_read_end += bytes_read;
    p_read_begin += bytes_read;
    bytes_remaining -= bytes_read;
	while (true)
    {
      auto buf_range = cut_json_obj(read_buf.begin(), it_read_end);
      if( std::distance(buf_range.first, buf_range.second) > 0)
      {
        try {
          std::string res = m_keychain_func(fc::json::from_string(std::string(buf_range.first, buf_range.second)));
          std::stringstream strbuf(std::ios_base::out);
          strbuf << res << std::endl;
          write(m_pipeout_desc, static_cast<const void*>(strbuf.str().c_str()), res.size());
        }
        catch(fc::exception& exc)
        {
          std::cerr << fc::json::to_pretty_string(fc::variant(json_error(0, exc.to_detail_string().c_str()))) << std::endl;
          std::string res = fc::json::to_pretty_string(fc::variant(json_error(0, exc.what())));
          std::stringstream strbuf(std::ios_base::out);
          strbuf << res << std::endl;
          write(m_pipeout_desc, static_cast<const void*>(strbuf.str().c_str()), res.size());
        }
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
  }
  return 0;
}

pipeline_parser::iter_range pipeline_parser::cut_json_obj(pipeline_parser::buf_iterator parse_begin, pipeline_parser::buf_iterator parse_end)
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
