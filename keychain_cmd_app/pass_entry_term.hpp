//
// Created by user on 05.06.18.
//

#ifndef KEYCHAINCMDAPP_PASS_ENTRY_TERM_HPP
#define KEYCHAINCMDAPP_PASS_ENTRY_TERM_HPP

#endif //KEYCHAINCMDAPP_PASS_ENTRY_TERM_HPP

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

#define MAX_KEYCODE_XORG            255  // максимально возможное кол-во кодов не м.б. более 255
#define DEV_INPUT_EVENT             std::string("/dev/input/")
#define PROC_BUS_INPUT_DEVICES      "/proc/bus/input/devices"

class pass_entry_term
{
public:
    pass_entry_term();
    ~pass_entry_term();
    std::wstring input_password();
private:
    bool map_translate();
    void ChangeKbProperty(XDeviceInfo *, Atom, Atom, int, unsigned char);
    bool OnKey (unsigned short, int, int, int, std::wstring&);
    unsigned int keyState(unsigned int);
    std::list<std::string> parse_device_file();

    Display* _display = NULL;

    KeySym  keysym[MAX_KEYCODE_XORG*2]; // max count of keycodes  for lowercase and uppercase
    int dev_cnt = 0;
    XDeviceInfo * dev_info;
    Atom device_enabled_prop, kbd_atom;
};

