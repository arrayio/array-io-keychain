#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <pwd.h>
#include <grp.h>

#define pid_Xorg "ps -eo pid,cmd|grep Xorg|awk '/-auth /{print $1}'"
#define user_Xorg "ps -eo uid,cmd |grep Xorg|awk '/-auth /{print $1}'"
#define keychain_service "keychain_service"
#define xhost "export XAUTHORITY=$(ps -eo cmd |grep Xorg|awk '/-auth /{print $6}');xhost +SI:localuser:" keychain_service
#define websocketd "/usr/local/bin/websocketd"
#define keychain "/usr/local/bin/keychain"


uid_t pipeof(const char * cmd)
{
    int pfd[2], res;

    if (pipe(pfd) == -1)  throw std::runtime_error("pipe()");
    switch (fork())
    {
        case (-1): throw std::runtime_error("fork()");
        case 0:
        {
            if (close(pfd[0]) == -1) throw std::runtime_error("close child");
            if (pfd[1] != STDOUT_FILENO) {
                if (dup2(pfd[1], STDOUT_FILENO) == -1) throw std::runtime_error("dup");
                if (close(pfd[1]) == -1) throw std::runtime_error("close child");
            }
            std::system(cmd);
            _exit(EXIT_SUCCESS);
        }
        default: break;
    }
    if (close(pfd[1]) == -1)  throw std::runtime_error("close parent");

    char buf[1024];
    res = read(pfd[0], &buf, sizeof(buf));
    std::string s = std::string(buf, buf+res);
    uid_t id = std::stoi(s);
    if (close(pfd[0]) == -1) throw std::runtime_error("close parent");

    return id;
}

int main() {

    struct passwd* pw = getpwnam(keychain_service);
    auto child = fork();
    switch (child)
    {
        case (-1): throw std::runtime_error("fork()");
        case 0:
        {
            initgroups(keychain_service, pw->pw_gid);
            if (setgid(pw->pw_gid) == -1) throw std::runtime_error("setgid()");
            if (setuid(pw->pw_uid) == -1) throw std::runtime_error("setuid()");
            execlp( websocketd,
                    websocketd,
                    "--port=16384", "--devconsole", "--passenv=DISPLAY", keychain,
                    (char *) NULL);
            throw std::runtime_error("execlp()");
        }
        default: break;
    }

    auto xuser = pipeof(user_Xorg);
    if (setuid(xuser== -1)) throw std::runtime_error("setuid()");

    pid_t prev = 0;
    while  (true)
    {
        auto pid = pipeof(pid_Xorg);
        if (pid !=  prev)
        {
            std::system (xhost);
            prev = pid;
        }
        sleep (10);
    }

    return 0;
}

