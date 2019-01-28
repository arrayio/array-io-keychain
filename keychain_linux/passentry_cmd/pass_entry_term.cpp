//
// Created by user on 05.06.18.
//
#include <sys/wait.h>
#include <boost/filesystem.hpp>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "pass_entry_term.hpp"
#include "cmd.hpp"
#include <crack.h>

#define path_ "./passentry_gui"
#define small  "/usr/local/share/cracklib/pw_small"
#define large  "/usr/local/share/cracklib/pw_dict"

pass_entry_term::pass_entry_term(bool confirm_) : confirm(confirm_)
{
    if (getresuid(&oruid, &oeuid, &osuid) == -1 )throw std::runtime_error("terminal: getresuid()");
    if (setresuid(oruid, oruid, osuid)    == -1 )throw std::runtime_error("terminal: setresuid()");

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
    setresuid(oruid, oeuid, osuid);
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
}


bool pass_entry_term::OnKey (unsigned short scancode, int shft, int cpslock, int nmlock, std::string& pass, const KeySym * map)
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

keychain_app::byte_seq_t pass_entry_term::fork_gui(const KeySym * map, const std::string& mes){
    int sockets[2];
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
                if (setresuid(oruid, oruid, oruid) == -1) throw std::runtime_error("GUI: setresuid()");
                execlp(path_, path_, (char *) NULL);
                throw std::runtime_error("execlp()");
            }
        default: break;
    }
    close(sockets[0]);
    send_gui(mes , sockets[1]);

    std::string pass = input_password(map, sockets[1]);
    close(sockets[1]);
    if (wait(NULL) == -1)   throw std::runtime_error("waiting gui");

    keychain_app::byte_seq_t vec(pass.begin(), pass.end());

    return vec;
};


std::string  pass_entry_term::input_password(const KeySym * map, int socket)
{
    std::vector<std::string> password(2);
    int line_edit = 0;

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
    auto gui = polling(socket);
    ChangeKbProperty(dev_info, kbd_atom, device_enabled_prop, dev_cnt, 0);

    capslock = keyState(XK_Caps_Lock);
    numlock = keyState(XK_Num_Lock);

    auto a = master::cmd<master::cmds::modify>(capslock, numlock, shift);
    auto mes = fc_light::json::to_string(fc_light::variant(static_cast<const master::cmd_base&>(a)));
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

    std::vector<int>  pass_len(2, 0);
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
                if (size % sizeof(input_event) !=0 ) break;
                if (size / sizeof(input_event) !=1 ) break;


                first_key = false;

                if (/*ev[0].value != ' ' &&*/ ev[1].type == EV_KEY )
                {
                    if (ev[1].value == 1)
                    {
                        if      (ev[1].code == KEY_LEFTSHIFT || ev[1].code == KEY_RIGHTSHIFT) shift = 1;
                        else if (ev[1].code == KEY_CAPSLOCK) capslock ^= 0x1;
                        else if (ev[1].code == KEY_NUMLOCK)  numlock ^= 0x1;
                        else if (ev[1].code == KEY_TAB && confirm )
                        {
                            line_edit ^= 0x1;
                            auto a = master::cmd<master::cmds::focus>(line_edit);
                            auto mes = fc_light::json::to_string(fc_light::variant(static_cast<const master::cmd_base&>(a)));
                            send_gui( mes, socket );
                        }
                        else if (gui.expert_mode) // advanced window in trx sign
                        {
                            if (ev[1].code == KEY_ENTER || ev[1].code == KEY_ESC)
                            {
                                auto v = master::cmd<master::cmds::close_expert_mode>();
                                mes = fc_light::json::to_string(fc_light::variant(v.base));
                                send_gui( mes, socket );
                            }
                        }
                        else if (confirm && ev[1].code == KEY_ESC) // create key
                        {
                            password[0].clear();
                            password[1].clear();
                            break;
                        }
                        else if (confirm && ev[1].code == KEY_ENTER) // create key
                        {
                            if (password[0] == password[1])  break;
                        }
                        else if (OnKey (ev[1].code, shift, capslock, numlock, password[line_edit], map)) break;
                    }
                    else if (ev[1].value == 0)
                    {
                        if ( (ev[1].code == KEY_LEFTSHIFT) || (ev[1].code == KEY_RIGHTSHIFT))  shift=0;
                    }
                    if ( (capslock | (numlock<<1) | (shift<<2)) != modify )
                    {
                        auto a = master::cmd<master::cmds::modify>(capslock, numlock, shift);
                        auto mes = fc_light::json::to_string(fc_light::variant(static_cast<const master::cmd_base&>(a)));
                        send_gui( mes, socket );
                    }
                    modify = capslock| (numlock<<1) | (shift<<2);
                }
            }
            FD_SET(kbd_id, &readfds);

            if (pass_len[line_edit] != password[line_edit].length())  // sending gui pass length
            {
                pass_len[line_edit] = password[line_edit].length();
                auto a = master::cmd<master::cmds::length>(pass_len[line_edit], line_edit);
                auto mes = fc_light::json::to_string(fc_light::variant(static_cast<const master::cmd_base&>(a)));
                send_gui( mes, socket );

                if (confirm)
                {
                    auto t = master::cmd<master::cmds::check>(password[0] == password[1]);
                    auto mes = fc_light::json::to_string(fc_light::variant(static_cast<const master::cmd_base&>(t)));
                    send_gui( mes, socket );

                    keychain_app::byte_seq_t vec(password[0].begin(), password[0].end());
                    if (vec.size())
                    {
                        const char * err = nullptr;
                        err = FascistCheck(vec.data(), large);
                        if (err )
                        {
                            err = FascistCheck(vec.data(), small);
                            if (err)
                            {
                                auto t = master::cmd<master::cmds::strength>(strength_te::weak);
                                auto mes = fc_light::json::to_string(fc_light::variant(static_cast<const master::cmd_base&>(t)));
                                send_gui( mes, socket );
                            }
                            else
                            {
                                auto t = master::cmd<master::cmds::strength>(strength_te::middle);
                                auto mes = fc_light::json::to_string(fc_light::variant(static_cast<const master::cmd_base&>(t)));
                                send_gui( mes, socket );
                            }
                        }
                        else{ //strong
                            auto t = master::cmd<master::cmds::strength>(strength_te::strong);
                            auto mes = fc_light::json::to_string(fc_light::variant(static_cast<const master::cmd_base&>(t)));
                            send_gui( mes, socket );
                        }

                    }
                }
            }

            gui.Select();  // polling gui

            if (gui.CancelButtonPressEvent)
            {
                gui.CancelButtonPressEvent = false;
                password[0].clear();
                password[1].clear();
                break;
            }

            if (gui.OkButtonPressEvent)
            {
                gui.OkButtonPressEvent = false;
                if (!confirm || (password[0] == password[1]))  break;
            }

            if (gui.focusEvent)
            {
                gui.focusEvent = false;
                line_edit = gui.line_edit;
            }
        }
        auto v = master::cmd<master::cmds::close>();
        mes = fc_light::json::to_string(fc_light::variant(v.base));
        send_gui( mes, socket );

        if (kbd_id != -1) ioctl(kbd_id, EVIOCGRAB, 0);
        for (auto dev : fd_list)  close(dev);
    }
    catch (const std::exception& e)
    {
        auto v = master::cmd<master::cmds::close>();
        mes = fc_light::json::to_string(fc_light::variant(v.base));
        send_gui( mes, socket );

        if (kbd_id != -1) ioctl(kbd_id, EVIOCGRAB, 0);
        for (auto dev : fd_list)  close(dev);
        password[0].clear();
        password[1].clear();
        throw;
    }
    return  password[line_edit];
}

void pass_entry_term::send_gui (std::string mes, int socket_gui )
{
    if (  write(socket_gui, mes.c_str(), mes.length()  )  != mes.length() )
        throw std::runtime_error("sending event to gui");
}

const map_translate_singleton& map_translate_singleton::instance(Display * d)
{
    static const map_translate_singleton instance(d);
    return instance;
}

map_translate_singleton::map_translate_singleton(Display * _display) // translate map keycode(Xorg)->keysym(Xorg)
{
    int  min_Xcode = 0, max_Xcode=0, gc;
    XkbStateRec state[1];
    char *names[XkbNumKbdGroups];
    char **pnames;
    XkbDescRec desc[1];

    memset(state, 0, sizeof(state));
    memset(desc, 0, sizeof(desc));

    XkbGetState(_display, XkbUseCoreKbd, state);

    desc->device_spec = XkbUseCoreKbd;
    XkbGetControls(_display, XkbGroupsWrapMask, desc);
    XkbGetNames(_display, XkbGroupNamesMask, desc);
    XGetAtomNames(_display, desc->names->groups, gc=desc->ctrls->num_groups, names);
    XkbFreeControls(desc, XkbGroupsWrapMask, True);
    XkbFreeNames(desc, XkbGroupNamesMask, True);

    XDisplayKeycodes(_display, &min_Xcode, &max_Xcode);

    std::string str (names[state->group]);
    if (str.substr(0, 7) != "English") throw std::runtime_error("Only English keyboard layout supported");

    pnames = names;
    for (; gc--; ++pnames)
        if (*pnames) {
            XFree(*pnames);
            *pnames = NULL;
        };

    if (max_Xcode > MAX_KEYCODE_XORG )  //  it is impossible
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


