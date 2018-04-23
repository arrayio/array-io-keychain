//
// Created by roman on 4/6/18.
//

#ifndef KEYCHAINAPP_PIPE_LINE_PARSER_HPP
#define KEYCHAINAPP_PIPE_LINE_PARSER_HPP

#include <vector>
#include <cstddef>
#include <string>

#include <json/json.hpp>

#include <sign_define.hpp>

namespace keychain_app {

struct json_error
{
    json_error(const char* str): error(str){}
    std::string error;
};

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

class pipeline_parser
{
public:
    pipeline_parser();
    int run();
private:
    using buf_type = std::vector<char>;
    using buf_iterator = buf_type::iterator;
    using iter_range = std::pair<buf_iterator, buf_iterator>;
    iter_range сut_json_obj(buf_iterator parse_begin, buf_iterator parse_end);//return pair(parse_end, parse_end)if no json detached, or pair<m_obj_begin, m_obj_end>
};

}

FC_REFLECT(keychain_app::json_error, (error))

#endif //KEYCHAINAPP_PIPE_LINE_PARSER_HPP
