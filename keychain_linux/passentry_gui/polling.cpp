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
