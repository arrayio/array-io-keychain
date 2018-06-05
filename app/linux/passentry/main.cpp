#include <list>
#include <vector>
#include <regex>
#include <iostream>
#include <fstream>
#include <X11/Xatom.h>
#include <linux/input.h>
#include <X11/extensions/XInput.h>
#include <X11/extensions/XInput2.h>
#include "keysym2ucs.h"
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/XKBlib.h>

#define MAX_KEYCODE_XORG 255  // максимально возможное кол-во кодов не м.б. более 255
#define DEV_INPUT_EVENT std::string("/dev/input/")
#define PROC_BUS_INPUT_DEVICES "/proc/bus/input/devices"
#define PASSWORD_MAXLEN  128

Display* _display = NULL;
char* _displayName = NULL;
long _password [PASSWORD_MAXLEN];
int  _passwordLen = 0;
int  min_Xcode = 0, max_Xcode=0;
KeySym  keysym[MAX_KEYCODE_XORG*2]; // max count of keycodes  for lowercase and uppercase

void CloseX (void);
bool OnKey (unsigned short k, int shft, int cpslock, int nmlock);
unsigned int keyState(unsigned int  iKey);
void ChangeKbProperty(
        XDeviceInfo * p_dev_info,
        Atom p_kbd_atom,
        Atom p_device_enabled_prop,
        int p_dev_cnt,
        unsigned char p_state
        );
bool map_translate();
int input_password();
std::list<std::string> parse_device_file();
bool RunMainDialog (void);


bool RunMainDialog (void)
{
    int dev_cnt = 0;
    XDeviceInfo * dev_info;
    Atom device_enabled_prop, kbd_atom;

    _display = XOpenDisplay (_displayName);
    if (!_display)
    {
        std::cout << "ERR Unable to open X display ";
        return (false);
    }
    atexit (CloseX);

    if (!map_translate())  return false;

    dev_info            = XListInputDevices( _display, &dev_cnt);
    kbd_atom            = XInternAtom(_display, XI_KEYBOARD, true);
    device_enabled_prop = XInternAtom(_display, "Device Enabled", false);

    ChangeKbProperty(dev_info, kbd_atom, device_enabled_prop, dev_cnt, 0);
    input_password() ;
    ChangeKbProperty(dev_info, kbd_atom, device_enabled_prop, dev_cnt, 1);
    XFreeDeviceList(dev_info);
}


bool OnKey (unsigned short scancode, int shft, int cpslock, int nmlock)
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

    if (scancode== KEY_ENTER) {
        return true;
    } else if (scancode == KEY_ESC) {
        _passwordLen = 0;
        return true;
    } else if (scancode == KEY_BACKSPACE || scancode == KEY_DELETE) {
        if (_passwordLen > 0)
            --_passwordLen;
    } else   {
        KeySym  sym = keysym[(scancode)*2 +(cpslock xor shft)];
        long unicode = keysym2ucs( sym);

        if (_passwordLen < PASSWORD_MAXLEN  && unicode !=-1)
            _password[_passwordLen++] = unicode;
    }
    return false;
}

void CloseX (void)
{
    if (_display)
        XCloseDisplay (_display);
    _display = NULL;
}


unsigned int keyState(unsigned int key_sym)
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


void ChangeKbProperty(
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

bool map_translate() // translate map keycode(Xorg)->keysym(Xorg)
{
    XkbStateRec state[1];

    memset(state, 0, sizeof(state));
    XkbGetState(_display, XkbUseCoreKbd, state);
    XDisplayKeycodes(_display, &min_Xcode, &max_Xcode);

    if (max_Xcode > MAX_KEYCODE_XORG )  // на всякий случай. такого быть не должно
    {
        std::cout<<" Maximum keycode must be less 255. Current value: "<<max_Xcode <<std::endl;
        return false;
    }

    for (int i=min_Xcode; i<=max_Xcode; i++ )
    {
        for (int shft=0; shft<2; shft++)
            keysym[(i-+min_Xcode)*2+shft]= XkbKeycodeToKeysym(_display, i, state->group, shft);
    }

    return true;
}

std::list<std::string> parse_device_file()
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


int input_password()
{
    std::list<std::string>  devices;
    std::vector<int> fd_list;
    fd_set readfds;
    struct input_event ev[64];
    int size = sizeof(struct input_event);
    int res, nfds=0, id = 0, kbd_id = -1;
    int capslock = 0, numlock = 0, shift = 0;
    struct timeval to ={0, 0};
    char name[256] = "Unknown";
    bool first_key = true;

    capslock = keyState(XK_Caps_Lock);
    numlock = keyState(XK_Num_Lock);

    FD_ZERO(&readfds);
    devices = parse_device_file();
    for (auto &sdev : devices)
    {
        id = open(sdev.c_str(), O_RDONLY);
        if (id > FD_SETSIZE || id == -1 ) break;
        if (id > nfds)  nfds = id + 1;

        fd_list.push_back(id);
        FD_SET(id, &readfds);
    }

    while (1)
    {
        res = select(nfds, &readfds, NULL, NULL, &to);
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
                            res = ioctl(kbd_id, EVIOCGNAME(sizeof(name)), name);
                            std::cout<<"Reading from :"<< name <<std::endl;
                            std::cout<<"Getting exclusive access: :";
                            res = ioctl(kbd_id, EVIOCGRAB, 1);
                            std::cout<<((res == 0) ? "SUCCESS" : "FAILURE")<<std::endl;
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
//                    printf ("Code[%d] \n", (ev[1].code));
                    if ( (ev[1].code == KEY_LEFTSHIFT) || (ev[1].code == KEY_RIGHTSHIFT))
                        shift = 1;
                    else if (ev[1].code == KEY_CAPSLOCK)
                        capslock ^= 0x1;
                    else if (ev[1].code == KEY_NUMLOCK)
                        numlock ^= 0x1;
                    else
                    {
                        if (OnKey (ev[1].code, shift, capslock, numlock) )
                           break;
                    }
                }else if (ev[1].value == 0)
                {
                    if ( (ev[1].code == KEY_LEFTSHIFT) || (ev[1].code == KEY_RIGHTSHIFT))
                        shift=0;
                }
            }
        }
        FD_SET(kbd_id, &readfds);
    }

    if (kbd_id != -1) res = ioctl(kbd_id, EVIOCGRAB, 1);
    for (auto dev : fd_list)  close(dev);
    return 1;
}


int main ()
{
    RunMainDialog();
    if (_passwordLen > 0)
    {
        std::cout << "password (unicode): ";
        for (int i = 0; i < _passwordLen; i++)
            printf("0x%lx ", _password[i]);
    }

    std::cout <<std::endl<< "OK ";

    return 0;
}

