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
#include <fc/reflect/reflect.hpp>
#include <fc/variant.hpp>
#include <graphene/chain/protocol/protocol.hpp>
#include <fc/io/json.hpp>

#define MAX_KEYCODE_XORG            255  // максимально возможное кол-во кодов не м.б. более 255
#define MAP_SIZE                    MAX_KEYCODE_XORG*2
#define DEV_INPUT_EVENT             std::string("/dev/input/")
#define PROC_BUS_INPUT_DEVICES      "/proc/bus/input/devices"

//namespace graphene{ namespace  chain { struct transaction;}}
enum cmd_enum {
    cmd_unknown = 0, cmd_rawtrx, cmd_close, cmd_modify, cmd_length, cmd_last
};

class pass_entry_term
{
public:
    pass_entry_term();
    ~pass_entry_term();
//    std::wstring fork_gui(const KeySym * map, const graphene::chain::transaction& );
    std::wstring fork_gui(const KeySym * map, const std::string& raw_trx );
    Display* _display = NULL;

    struct json_rawtrx
    {
        json_rawtrx(int cmd_, std::string rawtrx_): cmd(cmd_), rawtrx(rawtrx_.c_str()){};
        int cmd;
        fc::variant rawtrx;
    };
    struct json_close
    {
        json_close(int cmd_): cmd(cmd_){};
        int cmd;
    };
    struct json_modify
    {
        json_modify(std::string cmd_, int caps_, int num_, int shift_): cmd(cmd_), caps(caps_), num(num_), shift(shift_){};
        std::string cmd;
        int caps, num, shift;
    };
    struct json_length
    {
        json_length(int cmd_, int len_): cmd(cmd_), len(len_){}
        int cmd;
        fc::variant len;
    };
private:
    std::wstring input_password(const KeySym *, int);
    void ChangeKbProperty(XDeviceInfo *, Atom, Atom, int, unsigned char);
    bool OnKey (unsigned short, int, int, int, std::wstring&, const KeySym *);
    unsigned int keyState(unsigned int);
    std::list<std::string> parse_device_file();
    void send_gui (int, int);
    void send_gui (std::string, int);
    bool polling_gui(std::wstring& , int);

    int dev_cnt = 0;
    XDeviceInfo * dev_info;
    Atom device_enabled_prop, kbd_atom;
    uid_t oruid, oeuid, osuid; // original value
};


class map_translate_singletone
{
public:
    map_translate_singletone(Display* );
    static const map_translate_singletone& instance(Display*);
    const KeySym * map;
private:
    KeySym  keysym[MAP_SIZE]; // max count of keycodes  for lowercase and uppercase
};

FC_REFLECT(pass_entry_term::json_rawtrx, (cmd)(rawtrx))
FC_REFLECT(pass_entry_term::json_close,  (cmd))
FC_REFLECT(pass_entry_term::json_modify, (cmd)(caps)(num)(shift))
FC_REFLECT(pass_entry_term::json_length, (cmd)(len))

#endif //KEYCHAINCMDAPP_PASS_ENTRY_TERM_HPP