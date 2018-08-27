//
// Created by user on 30.06.18.
//
#include "polling.hpp"
#include "cmd.hpp"
#include <unistd.h>
#include <fc_light/exception/exception.hpp>

void polling::Select()
{
    fd_set readfds;
    struct timeval to ={0, 1000};
    int res;

    auto begin  = [this](buf_it &i) { if (i>=buf.end()) i=buf.begin(); return &(*i); };
    auto remain = [this](buf_it &i) { if (i>=buf.end()) i=buf.begin(); return std::distance(i, buf.end());};

    FD_ZERO(&readfds);
    FD_SET(socket, &readfds);
    res = select(socket +1, &readfds, NULL, NULL, &to);
    if (res == -1)  throw std::runtime_error("polling gui socketpair");

    if (FD_ISSET(socket, &readfds))
    {
        size_t cnt = read(socket, begin(it), remain(it));
        if (cnt ==-1 ) throw std::runtime_error("gui reading error");

        it += cnt;
        while (true)
        {
            auto range = cut_json_obj(buf.begin(), it );
            std::string str = std::string(range.first, range.second);
            if(std::distance(range.first, range.second) > 0)
            {
                parse(std::string(range.first, range.second));
                it = std::copy(range.second, it, buf.begin());
            }
            else break;
        }
    }
}

it_range polling::cut_json_obj(buf_it parse_begin, buf_it parse_end)
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
        return it_range(start_obj, it);
    }
    else
    {
        return it_range(parse_end, parse_end);
    }
}

void polling::parse(const std::string s)
{
    auto a = fc_light::json::from_string(s);
    try {
        auto cmd = a.as<slave::cmd_common>();
        auto cmd_map = slave::cmd_list_singletone::instance();
        auto p_func = cmd_map[cmd.cmd];
        (*p_func)(this, cmd.params);
    }
    catch (const std::exception &e) {throw std::runtime_error(e.what());}
    catch (const fc_light::exception &e) {throw std::runtime_error(e.what());}
};

