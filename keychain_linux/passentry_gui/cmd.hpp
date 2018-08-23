//
// Created by user on 23.06.18.
//
#ifndef KEYCHAINAPP_CMD_H
#define KEYCHAINAPP_CMD_H
#include <fc/reflect/reflect.hpp>
#include <fc/exception/exception.hpp>
//#include <graphene/chain/protocol/protocol.hpp>
#include <fc/io/json.hpp>
#include <iostream>
#include <vector>
#include <boost/hana.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/for_each.hpp>
#include <boost/hana/size.hpp>
#include "widget.hpp"

namespace  slave {
    enum struct cmds {unknown = 0, rawtrx, close, modify, length, last }; // to gui

    struct cmd_common {
        cmd_common(cmds cmd_ = cmds::unknown): cmd(cmd_){};
        cmds cmd;
        fc::variant params;
    };
    struct cmd_base {
        cmd_base(cmds cmd_) : cmd(cmd_) {};
        cmds cmd;
        virtual void operator()(Widget* w, const fc::variant&) const = 0;
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

namespace master
{
    enum struct cmds {unknown = 0, ok, cancel, last};

    template<cmds cmd_>
    struct cmd {};
    template<>
    struct cmd<cmds::cancel> {
        cmd():base(){}
        struct base_t {base_t(){base_t::cmd=cmds::cancel;} cmds cmd;} base;
    };
    template<>
    struct cmd<cmds::ok> {
        cmd():base(){}
        struct base_t {base_t(){base_t::cmd=cmds::ok;} cmds cmd;} base;
    };
}

FC_REFLECT_ENUM(slave::cmds, (unknown)(rawtrx)(close)(modify)(length)(last))
FC_REFLECT(slave::cmd_common, (cmd)(params))

FC_REFLECT_ENUM(master::cmds, (unknown)(ok)(cancel)(last))
FC_REFLECT(master::cmd<master::cmds::ok>::base_t, (cmd))
FC_REFLECT(master::cmd<master::cmds::cancel>::base_t, (cmd))
#endif //KEYCHAINAPP_CMD_H
