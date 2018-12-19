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
    auto child = fork();
    switch (child)
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
    int status;
    if (waitpid(child, &status, WNOHANG) == -1)   throw std::runtime_error("wait()");

    pid_t prev = 0;
    while  (true)
    {
        char buf[512];
        FILE *pipe = popen("pidof -s Xorg", "r");
        fgets(buf, sizeof(buf), pipe);
        pclose( pipe );
        auto cur = strtoul(buf, NULL, 10);

        if (cur !=  prev)
        {
            std::system (xhost);
            prev = cur;
        }
        sleep (10);
    }

    return 0;
}

