//
// Created by user on 23.06.18.
//
#ifndef KEYCHAINAPP_CMD_H
#define KEYCHAINAPP_CMD_H
#include <fc/reflect/reflect.hpp>
#include <graphene/chain/protocol/protocol.hpp>
#include <fc/io/json.hpp>
#include <iostream>
#include <vector>

#undef Bool //NOTE: Xlib contains #define Bool, that causes error compilation hana
#undef None //NOTE: Xlib contains #define None, that causes error compilation hana
#include <boost/hana.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/for_each.hpp>
#include <boost/hana/size.hpp>
#include "polling.hpp"

namespace  slave {
    enum struct cmds {unknown = 0, ok, cancel, last}; //from gui

    struct cmd_common {
        cmd_common(cmds cmd_ = cmds::unknown): cmd(cmd_){};
        cmds cmd;
        fc::variant params;
    };
    struct cmd_base {
        cmd_base(cmds cmd_) : cmd(cmd_) {};
        cmds cmd;
        virtual void operator()(polling*, const fc::variant&) const = 0;
        virtual ~cmd_base() {};
    };
    struct cmd_list_singletone
    {
    public:
        static const cmd_list_singletone& instance();
        const std::shared_ptr<cmd_base> operator[](cmds) const;
    private:
        cmd_list_singletone();
        std::vector<std::shared_ptr<cmd_base>> cmd_list;
    };
}

namespace  master {
    enum struct cmds {unknown = 0, rawtrx, close, modify, length, last};

    struct cmd_base {
        cmd_base(): cmd(cmds::unknown){};
        cmds cmd;
        fc::variant params;
    };
    template<cmds cmd_>
    struct cmd : cmd_base {};

    template<>
    struct cmd<cmds::rawtrx> : cmd_base{
        cmd(std::string rawtrx_): cmd_base(), rawtrx(rawtrx_){
            cmd_base::cmd = cmds::rawtrx;
            params = fc::variant(rawtrx);
        };
        struct rawtrx_t { rawtrx_t(std::string s):rawtrx(s){}
            fc::variant rawtrx;
        };
        rawtrx_t rawtrx;
    };

    template<>
    struct cmd<cmds::close> {
        cmd():base(){}
        struct base_t {base_t(){base_t::cmd=cmds::close;} cmds cmd;} base;
    };

    template<>
    struct cmd<cmds::modify> : cmd_base {
        cmd(bool caps_, bool num_, bool shift_): cmd_base(), modify(caps_, num_, shift_){
            cmd_base::cmd = cmds::modify;
            params = fc::variant(modify);
        };
        struct modify_t { modify_t(bool caps_, bool num_, bool shift_):caps(caps_), num(num_), shift(shift_){}
            fc::variant caps, num, shift;
        }  modify;
    };

    template<>
    struct cmd<cmds::length> : cmd_base{
        cmd(int length_): cmd_base(), length(length_){
            cmd_base::cmd = cmds::length;
            params = fc::variant(length);
        };
        struct length_t { length_t(int l):len(l){}
            fc::variant len;
        };
        length_t length;
    };
}

FC_REFLECT_ENUM(master::cmds, (unknown)(rawtrx)(close)(modify)(length)(last))
FC_REFLECT(master::cmd_base, (cmd)(params))
FC_REFLECT(master::cmd<master::cmds::rawtrx>::rawtrx_t, (rawtrx))
FC_REFLECT(master::cmd<master::cmds::close>::base_t, (cmd))
FC_REFLECT(master::cmd<master::cmds::modify>::modify_t, (caps)(num)(shift))
FC_REFLECT(master::cmd<master::cmds::length>::length_t, (len))

FC_REFLECT_ENUM(slave::cmds, (unknown)(ok)(cancel)(last))
FC_REFLECT(slave::cmd_common, (cmd)(params))

#endif //KEYCHAINAPP_CMD_H
