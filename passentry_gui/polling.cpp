#include <sys/socket.h>
#include <unistd.h>
#include "polling.hpp"

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
        char buf[1024];
        read(STDIN_FILENO, buf, 1024);
        //QString result =buf;
        emit rx(std::string(buf));
    }
    emit Polling::poll();
}

void Polling::parse(const std::string& s)
{
    auto a = fc::variant(s);
    auto cmd = a.as<gui::cmd_common>();
    cmd_map = gui::cmd_list_singletone::instance();
    auto p_func = cmd_map[cmd.cmd];
    //auto res = (*p_func)(cmd.params, cmd.id);
    std::cout << "pas_len="<< (*p_func)(cmd.params) << std::endl;
}
