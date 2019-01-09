//
// Created by user on 05.06.18.
//

#ifndef KEYCHAINCMDAPP_PASS_ENTRY_TERM_HPP
#define KEYCHAINCMDAPP_PASS_ENTRY_TERM_HPP
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
#include <keychain_lib/keychain_wrapper.hpp>
#include <cmd.hpp>


#define MAX_KEYCODE_XORG            255  // максимально возможное кол-во кодов не м.б. более 255
#define MAP_SIZE                    MAX_KEYCODE_XORG*2
#define DEV_INPUT_EVENT             std::string("/dev/input/")
#define PROC_BUS_INPUT_DEVICES      "/proc/bus/input/devices"

class pass_entry_term
{
public:
    pass_entry_term();
    ~pass_entry_term();

    keychain_app::byte_seq_t fork_gui(const KeySym*, const std::string&, master::cmds  );
    Display* _display = NULL;
private:
    std::wstring input_password(const KeySym *, int);
    void ChangeKbProperty(XDeviceInfo *, Atom, Atom, int, unsigned char);
    bool OnKey (unsigned short, int, int, int, std::wstring&, const KeySym *);
    unsigned int keyState(unsigned int);
    std::list<std::string> parse_device_file();
    void send_gui (std::string, int);

    int dev_cnt = 0;
    XDeviceInfo * dev_info;
    Atom device_enabled_prop, kbd_atom;
    uid_t oruid, oeuid, osuid; // original value
};


class map_translate_singleton
{
public:
  map_translate_singleton(Display* );
    static const map_translate_singleton& instance(Display*);
    const KeySym * map;
private:
    KeySym  keysym[MAP_SIZE]; // max count of keycodes  for lowercase and uppercase
};

#endif //KEYCHAINCMDAPP_PASS_ENTRY_TERM_HPP