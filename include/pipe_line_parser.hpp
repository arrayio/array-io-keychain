//
// Created by roman on 4/6/18.
//

#ifndef KEYCHAINAPP_PIPE_LINE_PARSER_HPP
#define KEYCHAINAPP_PIPE_LINE_PARSER_HPP

#include <vector>
#include <cstddef>

namespace keychain_app
{

namespace json_parse
{
  static const char* LBRACE = "{";
  static const char* RBRACE = "}";

  namespace json_keys
  {
    static const char* COMMAND = "command";
    static const char* PARAMS = "params";
    namespace command_params
    {
      static const char* CHAINID = "chainid";
      static const char* HDPATH = "hdpath";
      static const char* TRANSACTION_HASH = "transaction-hash";
      static const char* KEYFILE = "keyfile";
    }
  }

}

class pipe_line_parser
{
public:
    pipe_line_parser();
    int run();
private:
    using buf_type = std::vector<char>;
    using buf_iterator = buf_type::iterator;
    using iter_range = std::pair<buf_iterator, buf_iterator>;
    buf_type m_buf;
    iter_range сut_json_obj();//return pair<end, end> if no json detached, or pair<m_obj_begin, m_obj_end>
    buf_type::value_type* m_current;
    buf_iterator m_obj_begin;
    size_t m_brace_count;
};

}

#endif //KEYCHAINAPP_PIPE_LINE_PARSER_HPP
