#ifndef KEYCHAIN_CMDAPP_POLLING_H
#define KEYCHAIN_CMDAPP_POLLING_H

#include <vector>
#include <string>

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

using buf_type = std::vector<char>;
using buf_it = buf_type::iterator;
using it_range = std::pair<buf_it, buf_it>;

class polling
{
public:
    polling(int socket_): socket(socket_), OkButtonPressEvent(false), CancelButtonPressEvent(false),
    focusEvent(false), line_edit(0), expert_mode(false)
    {
        buf.resize(1024);
        it = buf.begin();
    }
    void Select();
    bool OkButtonPressEvent, CancelButtonPressEvent, focusEvent, expert_mode;
    int line_edit;
private:
    buf_it it;
    buf_type buf;
    int socket;
    it_range cut_json_obj(buf_it, buf_it);
    void parse(const std::string s);
};

#endif //KEYCHAIN_CMDAPP_POLLING_H