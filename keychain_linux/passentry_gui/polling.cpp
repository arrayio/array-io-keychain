#include <sys/socket.h>
#include <unistd.h>
#include "polling.hpp"
Q_DECLARE_METATYPE(std::string)

void Polling::Select()
{
    fd_set readfds;
    struct timeval to ={0, 1000};
    int res;

    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);
    res = select(STDIN_FILENO+1, &readfds, NULL, NULL, &to);
    if (res == -1) emit Polling::err();

    if (FD_ISSET(STDIN_FILENO, &readfds))
    {
        size_t cnt = read(STDIN_FILENO, pbuf, remain);
        if (cnt ==-1 ) throw std::runtime_error("gui reading error");

        pbuf += cnt;
        end += cnt;
        remain -= cnt;
        bool found_json = true;
        while (found_json )
        {
            auto range = cut_json_obj(buf.begin(), end );
            if(std::distance(range.first, range.second) > 0)
            {
                emit rx(std::string(range.first, range.second));
                auto cur = std::copy(range.second, end, buf.begin());
                std::for_each(cur, end, [](buf_type::value_type &val) {val = 0x00;});
                end = cur;
                pbuf = buf.data() + std::distance(buf.begin(), end);
                remain = std::distance(end, buf.end());
            }
            else
                found_json = false;
        }
    }
    emit Polling::poll();
}


iter_range Polling::cut_json_obj(buf_iterator parse_begin, buf_iterator parse_end)
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
        return iter_range(start_obj, it);
    }
    else
    {
        return iter_range(parse_end, parse_end);
    }
}



while (true)
{
auto buf_range = cut_json_obj(read_buf.begin(), it_read_end);
if(
std::distance(buf_range.first, buf_range.second) > 0)
{
}
}


