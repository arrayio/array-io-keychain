#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <pwd.h>
#include <grp.h>

#define user "keychain_service"
#define xhost "xhost +SI:localuser:" user
#define websocketd "/home/user/ws/websocketd"
#define keychain "/home/user/ws/keychain"


int main() {

    struct passwd* pw = getpwnam(user);
    std::system (xhost);
    switch (fork())
    {
        case (-1): throw std::runtime_error("fork()");
        case 0:
        {
            initgroups(user,pw->pw_gid);
            if (setgid(pw->pw_gid) == -1) throw std::runtime_error("setgid()");
            if (setuid(pw->pw_uid) == -1) throw std::runtime_error("setuid()");
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

