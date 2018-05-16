//
// Created by roman on 4/6/18.
//

#ifndef KEYCHAINAPP_PIPE_LINE_PARSER_HPP
#define KEYCHAINAPP_PIPE_LINE_PARSER_HPP

#include <vector>
#include <cstddef>
#include <string>

#include <sign_define.hpp>

namespace keychain_app {

namespace json_parser
{
static const char LBRACE = '{';
static const char RBRACE = '}';
static const char SPACE = ' ';
static const char LF = '\n';
static const char CR = '\r';
static const char TAB = '\t';
static const char VTAB = '\v';
static const char NP = '\f';
}

using keychain_invoke_f = std::function<void(const fc::variant&)>;

class pipeline_parser
{
public:
    pipeline_parser(keychain_invoke_f&& keychain_func, FILE* pd);
    int run();
private:
    keychain_invoke_f m_keychain_func;
    FILE* m_pd;
    using buf_type = std::vector<char>;
    using buf_iterator = buf_type::iterator;
    using iter_range = std::pair<buf_iterator, buf_iterator>;
    iter_range сut_json_obj(buf_iterator parse_begin, buf_iterator parse_end);//return pair(parse_end, parse_end)if no json detached, or pair<m_obj_begin, m_obj_end>
};

}

#endif //KEYCHAINAPP_PIPE_LINE_PARSER_HPP
