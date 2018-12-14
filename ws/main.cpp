#include <iostream>
#include <unistd.h>
#include <fstream>
#include <sys/wait.h>

#define get_uid "id -u keychain_service"
#define get_gid "getent group input | awk -F: '{printf $3}'"
#define xhost "xhost +SI:localuser:keychain_service"
#define websocketd "/home/user/ws/websocketd"
#define keychain "/home/user/ws/keychain"


int main() {

    int pfd[2], res;
    uid_t uid = 0 , gid;
    char buf[1024];
    std::string s;

    if (pipe(pfd) == -1)  throw std::runtime_error("pipe() 1");
    switch (fork())
    {
        case (-1): throw std::runtime_error("fork() 1");
        case 0:
        {
            if (close(pfd[0]) == -1) throw std::runtime_error("close child(0) 1");
            if (pfd[1] != STDOUT_FILENO) {
                if (dup2(pfd[1], STDOUT_FILENO) == -1) throw std::runtime_error("dup2 1");
                if (close(pfd[1]) == -1) throw std::runtime_error("close child(1) 1");
            }
            std::system(get_uid);
            _exit(EXIT_SUCCESS);
        }
        default: break;
    }
    if (close(pfd[1]) == -1)  throw std::runtime_error("close parent(1) 1");

    res = read(pfd[0], &buf, sizeof(buf));
    s = std::string(buf, buf+res);
    uid = std::stoi(s);
    if (close(pfd[0]) == -1) throw std::runtime_error("close parent(0) 1");

    if (pipe(pfd) == -1)  throw std::runtime_error("pipe() 2");
    switch (fork())
    {
        case (-1): throw std::runtime_error("fork() 2");
        case 0:
        {
            if (close(pfd[0]) == -1) throw std::runtime_error("close child(0) 2");
            if (pfd[1] != STDOUT_FILENO) {
                if (dup2(pfd[1], STDOUT_FILENO) == -1) throw std::runtime_error("dup2 2");
                if (close(pfd[1]) == -1) throw std::runtime_error("close child(1) 2");
            }
            std::system(get_gid);
            _exit(EXIT_SUCCESS);
        }
        default: break;
    }
    if (close(pfd[1]) == -1)  throw std::runtime_error("close parent(1) 2");

    res = read(pfd[0], &buf, sizeof(buf));
    s = std::string(buf, buf+res);
    gid = std::stoi(s);
    if (close(pfd[0]) == -1) throw std::runtime_error("close parent(0) 2");

    std::system (xhost);
    switch (fork())
    {
        case (-1): throw std::runtime_error("fork() 3");
        case 0:
        {
            if (setregid(gid, gid) == -1) throw std::runtime_error("setregid()");
            if (setresuid(uid, uid, uid) == -1) throw std::runtime_error("setresuid()");
            execlp( websocketd,
                    websocketd,
                    "--port=8080", "--devconsole", "--passenv=DISPLAY", keychain,
                    (char *) NULL);
            throw std::runtime_error("execlp()");
        }
        default: break;
    }
    if (wait(NULL) == -1)   throw std::runtime_error("wait()");
    return 0;
}

