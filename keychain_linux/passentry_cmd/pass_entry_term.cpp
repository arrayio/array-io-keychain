//
// Created by user on 05.06.18.
//
#include <sys/wait.h>
#include <boost/filesystem.hpp>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "pass_entry_term.hpp"
#include "cmd.hpp"
#include "polling.hpp"

#define path_  "/home/user/CLionProjects/array-io-keychain/cmake-debug-build/keychain_linux/passentry_gui/passentry_gui"

pass_entry_term::pass_entry_term() : passClearOnExit(false), closeEvent (false)
{
    uid_t ruid, euid, suid;

    if (getresuid(&oruid, &oeuid, &osuid) == -1 )throw std::runtime_error("terminal: getresuid()");
//    std::cout<<"terminal - ruid: "<< oruid <<" euid: "<<oeuid<<" suid: "<<osuid<< std::endl;
    if (setresuid(oruid, oruid, osuid)    == -1 )throw std::runtime_error("terminal: setresuid()");
    if (getresuid(&ruid, &euid, &suid)    == -1 )throw std::runtime_error("terminal: getresuid()");
//    std::cout<<"terminal - ruid: "<< ruid <<" euid: "<<euid<<" suid: "<<suid<< std::endl;

    char* _displayName = NULL;
    _display = XOpenDisplay (_displayName);
    if (!_display)
        throw std::runtime_error("ERR Unable to open X display");
    dev_info            = XListInputDevices( _display, &dev_cnt);
    kbd_atom            = XInternAtom(_display, XI_KEYBOARD, true);
    device_enabled_prop = XInternAtom(_display, "Device Enabled", false);
}

pass_entry_term::~pass_entry_term()
{
    ChangeKbProperty(dev_info, kbd_atom, device_enabled_prop, dev_cnt, 1);
    XFreeDeviceList(dev_info);

    if (_display)
        XCloseDisplay (_display);
    _display = NULL;

    uid_t ruid, euid, suid;
    setresuid(oruid, oeuid, osuid);
    getresuid(&ruid, &euid, &suid);
//    std::cout<<"terminal - ruid: "<< ruid <<" euid: "<<euid<<" suid: "<<suid<< std::endl;
}

void pass_entry_term::ChangeKbProperty(
        XDeviceInfo * p_dev_info,
        Atom p_kbd_atom,
        Atom p_device_enabled_prop,
        int p_dev_cnt,
        unsigned char p_state
)
{
    unsigned char st = p_state;

    for (auto i = 0;i < p_dev_cnt; i++)
    {
        if (p_dev_info[i].type == p_kbd_atom && p_dev_info[i].use == IsXExtensionKeyboard )
            XIChangeProperty( _display,
                              p_dev_info[i].id,
                              p_device_enabled_prop,
                              XA_INTEGER,
                              8,
                              XIPropModeReplace,
                              &st,
                              1
            );
    }
//    std::cout << "XIChangeProperty: keyboard " << (st==1 ? "ENABLE":"DISABLE") << std::endl;
}


bool pass_entry_term::OnKey (unsigned short scancode, int shft, int cpslock, int nmlock, std::wstring& pass, const KeySym * map)
{
    // HACK: manual NumPad processing (It is right only for English keyboard layout)
    switch (scancode)
    {
        case KEY_KP0: scancode = KEY_0; cpslock=0; shft=0; break;
        case KEY_KP1: scancode = KEY_1; cpslock=0; shft=0; break;
        case KEY_KP2: scancode = KEY_2; cpslock=0; shft=0; break;
        case KEY_KP3: scancode = KEY_3; cpslock=0; shft=0; break;
        case KEY_KP4: scancode = KEY_4; cpslock=0; shft=0; break;
        case KEY_KP5: scancode = KEY_5; cpslock=0; shft=0; break;
        case KEY_KP6: scancode = KEY_6; cpslock=0; shft=0; break;
        case KEY_KP7: scancode = KEY_7; cpslock=0; shft=0; break;
        case KEY_KP8: scancode = KEY_8; cpslock=0; shft=0; break;
        case KEY_KP9: scancode = KEY_9; cpslock=0; shft=0; break;

        case KEY_KPSLASH:       scancode = KEY_SLASH; cpslock=0; shft=0; break;
        case KEY_KPASTERISK:    scancode = KEY_8;     cpslock=0; shft=1; break;
        case KEY_KPMINUS:       scancode = KEY_MINUS; cpslock=0; shft=0; break;
        case KEY_KPPLUS:        scancode = KEY_EQUAL; cpslock=0; shft=1; break;
        case KEY_KPDOT:         scancode = KEY_DOT;   cpslock=0; shft=0; break;
        case KEY_KPENTER:       scancode = KEY_ENTER; cpslock=0; shft=0; break;
    }
    //for this range the capslock are not effected
    if ( (scancode >= KEY_1 && scancode  <= KEY_EQUAL) || (scancode == KEY_GRAVE ) )
        cpslock = 0;

    switch (scancode)
    {
        case KEY_ENTER:     return true;
        case KEY_ESC:       pass.clear();    return true;;
        case KEY_BACKSPACE: if (!pass.empty()) pass.pop_back(); break;
        case KEY_DELETE:    if (!pass.empty()) pass.pop_back(); break;
        default:{
                    int code = (scancode)*2 +(cpslock xor shft);
                    if (code < MAP_SIZE)
                    {
                        KeySym  sym = map[code];
                        long unicode = keysym2ucs( sym);

                        if ( unicode !=-1)
                        {
                            wchar_t w = static_cast<wchar_t>(unicode);
                            pass += w;
                        }
                    }
                    else
                        throw std::runtime_error("OnKey: map size exceeded");
                }
    }
    return false;
}

unsigned int pass_entry_term::keyState(unsigned int key_sym)
{
    unsigned int     map_mask =0, entity_mask=0;
    Window           wDummy1, wDummy2;
    int              iDummy3, iDummy4, iDummy5, iDummy6, i;
    XModifierKeymap* map;
    KeyCode          keyCode;

    keyCode = XKeysymToKeycode(_display, key_sym);
    map = XGetModifierMapping(_display);

    if (keyCode == NoSymbol)
        return (0);

    for (i = 0; i < 8; i++)
        if (map->modifiermap[map->max_keypermod * i] == keyCode)
            map_mask = 1 << i;

    XQueryPointer(_display, DefaultRootWindow(_display),  &wDummy1, &wDummy2, &iDummy3, &iDummy4, &iDummy5, &iDummy6, &entity_mask);
    XFreeModifiermap(map);

    return ((entity_mask & map_mask) != 0);
}

std::list<std::string> pass_entry_term::parse_device_file()
{
    std::ifstream f_dev (PROC_BUS_INPUT_DEVICES);
    std::list<std::string>  devices;
    const int buf_size = 1000;
    char buf[buf_size];

    while(!f_dev.eof())
    {
        f_dev.getline(buf, buf_size);
        std::string line(buf);
        std::regex rx("H: Handlers=.*kbd.* event[0-9]+");
        std::regex_match(line.begin(), line.end(), rx);
        if (std::regex_search(line.begin(), line.end(), rx) )
        {
            std::regex rx_event("event[0-9]+");
            auto begin = std::sregex_iterator(line.begin(), line.end(), rx_event);
            std::smatch value = *begin;
            static_assert(std::is_same<std::string, decltype(value.str())>::value , "error found ");
            devices.push_back( DEV_INPUT_EVENT + value.str() );
        }
    }
    return std::move( devices);
}

std::wstring pass_entry_term::fork_gui(const KeySym * map, const std::string& raw_trx ){
//std::wstring pass_entry_term::fork_gui(const KeySym * map, const graphene::chain::transaction& trx) {
    int sockets[2];
    uid_t ruid, euid, suid;
    if (getresuid(&ruid, &euid, &suid) == -1)  throw std::runtime_error("getresuid()");

    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockets) < 0)   throw std::runtime_error("opening stream socket pair");
    switch (fork())
    {
        case (-1): throw std::runtime_error("fork()");
        case 0:{
                close(sockets[1]);
                if (sockets[0] != STDIN_FILENO)
                {
                    if (dup2(sockets[0], STDIN_FILENO) == -1) throw std::runtime_error("dup2");
                    if (close(sockets[0]) == -1) throw std::runtime_error("close socket[0]");
                }
                if (setresuid(ruid, ruid, ruid) == -1) throw std::runtime_error("GUI: setresuid()");
                if (getresuid(&ruid, &euid, &suid) == -1) throw std::runtime_error("GUI: getresuid()");
//                std::cout<<"GUI - ruid: "<< ruid <<" euid: "<<euid<<" suid: "<<suid<< std::endl;
                execlp(path_, path_, (char *) NULL);
                throw std::runtime_error("execlp()");
            }
        default: break;
    }
    close(sockets[0]);
    auto a = gui::cmd<gui::cmds::rawtrx>(raw_trx);
    auto mes = fc::json::to_pretty_string(fc::variant(static_cast<const gui::cmd_base&>(a)));
    send_gui(mes , sockets[1]);

    std::wstring s = input_password(map, sockets[1]);
    close(sockets[1]);
    if (wait(NULL) == -1)   throw std::runtime_error("waiting gui");
    return  s;
};


std::wstring pass_entry_term::input_password(const KeySym * map, int socket)
{
    std::wstring password;

    std::list<std::string>  devices;
    std::vector<int> fd_list;
    fd_set readfds;
    struct input_event ev[64];
    int size = sizeof(struct input_event);
    int res, nfds=0, id = 0, kbd_id = -1;
    int capslock = 0, numlock = 0, shift = 0, modify = 0;
    struct timeval to ={0, 0};
    char name[256] = "Unknown";
    bool first_key = true;
    auto gui = polling(socket, *this);
    ChangeKbProperty(dev_info, kbd_atom, device_enabled_prop, dev_cnt, 0);

    capslock = keyState(XK_Caps_Lock);
    numlock = keyState(XK_Num_Lock);

    auto a = gui::cmd<gui::cmds::modify>(capslock, numlock, shift);
    auto mes = fc::json::to_pretty_string(fc::variant(static_cast<const gui::cmd_base&>(a)));
    send_gui( mes, socket );

    FD_ZERO(&readfds);
    devices = parse_device_file();
    for (auto &sdev : devices)
    {
        id = open(sdev.c_str(), O_RDONLY);
        if (id > FD_SETSIZE || id == -1 ) continue;
        if (id > nfds)  nfds = id + 1;

        fd_list.push_back(id);
        FD_SET(id, &readfds);
    }

    int pass_len = 0;
    try
    {
        while (1)
        {
            res = select(nfds, &readfds, NULL, NULL, &to); // polling keyboard
            if (res == -1)  break;

            if (kbd_id == -1)
            {
                for (id = 0; id < fd_list.size(); id++)
                {
                    if (FD_ISSET(fd_list[id], &readfds) )
                    {
                        if ((res = read(fd_list[id], ev, size * 64)) < size)    break;
                        if (ev[1].type == EV_KEY )
                        {
                            if ( ev[1].code <= 255)
                            {
                                kbd_id = fd_list[id];
                                if (ioctl(kbd_id, EVIOCGRAB, 1) != 0) throw std::runtime_error("cannot get exclusive access to keyboard");
                                break;
                            }
                        }
                    }
                    FD_SET(fd_list[id], &readfds);
                }
            }

            if (kbd_id != -1 && FD_ISSET(kbd_id, &readfds) )
            {
                FD_ZERO(&readfds);
                if (!first_key)
                {
                    if ((res = read(kbd_id, ev, size * 64)) < size)
                        break;
                }
                first_key = false;

                if (ev[0].value != ' ' && ev[1].type == EV_KEY )
                {
                    if (ev[1].value == 1)
                    {
                        if      (ev[1].code == KEY_LEFTSHIFT || ev[1].code == KEY_RIGHTSHIFT) shift = 1;
                        else if (ev[1].code == KEY_CAPSLOCK) capslock ^= 0x1;
                        else if (ev[1].code == KEY_NUMLOCK)  numlock ^= 0x1;
                        else if (OnKey (ev[1].code, shift, capslock, numlock, password, map))
                        {
                            auto v = gui::cmd<gui::cmds::close>();
                            auto mes = fc::json::to_pretty_string(fc::variant(v.base));
                            send_gui( mes, socket );
                            break;
                        }
                    }
                    else if (ev[1].value == 0)
                    {
                        if ( (ev[1].code == KEY_LEFTSHIFT) || (ev[1].code == KEY_RIGHTSHIFT))  shift=0;
                    }
                    if ( (capslock | (numlock<<1) | (shift<<2)) != modify )
                    {
                        auto a = gui::cmd<gui::cmds::modify>(capslock, numlock, shift);
                        auto mes = fc::json::to_pretty_string(fc::variant(static_cast<const gui::cmd_base&>(a)));
                        send_gui( mes, socket );
                    }
                    modify = capslock| (numlock<<1) | (shift<<2);
                }
            }
            FD_SET(kbd_id, &readfds);

            if (pass_len != password.length())  // sending gui pass length
            {
                pass_len = password.length();
                auto a = gui::cmd<gui::cmds::length>(pass_len);
                auto mes = fc::json::to_pretty_string(fc::variant(static_cast<const gui::cmd_base&>(a)));
                send_gui( mes, socket );
            }
            gui.Select();  // polling gui
            if (passClearOnExit) password.clear();
            if (closeEvent) break;
        }
        if (kbd_id != -1) ioctl(kbd_id, EVIOCGRAB, 0);
        for (auto dev : fd_list)  close(dev);
    }
    catch (const std::exception& e)
    {
        if (kbd_id != -1) ioctl(kbd_id, EVIOCGRAB, 0);
        for (auto dev : fd_list)  close(dev);
        password.clear();
        throw;
    }
    return password;
}

void pass_entry_term::send_gui (std::string mes, int socket_gui )
{
    if (  write(socket_gui, mes.c_str(), mes.length()  )  != mes.length() )
        throw std::runtime_error("sending event to gui");
}

const map_translate_singletone& map_translate_singletone::instance(Display * d)
{
    static const map_translate_singletone instance(d);
    return instance;
}

map_translate_singletone::map_translate_singletone(Display * _display) // translate map keycode(Xorg)->keysym(Xorg)
{
    int  min_Xcode = 0, max_Xcode=0;
    XkbStateRec state[1];

    memset(state, 0, sizeof(state));
    XkbGetState(_display, XkbUseCoreKbd, state);
    XDisplayKeycodes(_display, &min_Xcode, &max_Xcode);

    XkbDescRec desc[1];
    memset(desc, 0, sizeof(desc));
    desc->device_spec = XkbUseCoreKbd;
    XkbGetControls(_display, XkbGroupsWrapMask, desc);
    XkbGetNames(_display, XkbAllNamesMask, desc    );

    if (desc[0].names->groups[state->group] != 219) throw std::runtime_error("Only English(American) keyboard layout supported");

    if (max_Xcode > MAX_KEYCODE_XORG )  // на всякий случай. такого быть не должно
    {
        std::cout<<" Maximum keycode must be less 255. Current value: "<<max_Xcode <<std::endl;
        throw std::runtime_error("Maximum keycode must be less 255.");
    }

    for (int i=min_Xcode; i<=max_Xcode; i++ )
    {
        for (int shft=0; shft<2; shft++)
        {
            int code = (i-+min_Xcode)*2+shft;
            if (code < MAP_SIZE)
                keysym[code]= XkbKeycodeToKeysym(_display, i, state->group, shft);
            else
                throw std::runtime_error("Key map translate: map size exceeded");
        }

    }
    map = keysym;
}


