//
// Created by user on 23.06.18.
//
#ifndef KEYCHAINAPP_CMD_H
#define KEYCHAINAPP_CMD_H
#include <fc_light/reflect/reflect.hpp>
#include <fc_light/reflect/variant.hpp>
#include <fc_light/exception/exception.hpp>
#include <fc_light/io/json.hpp>
#include <iostream>
#include <vector>
#include <boost/hana.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/for_each.hpp>
#include <boost/hana/size.hpp>
#include "widget.hpp"

namespace  slave {
    enum struct cmds {unknown = 0, rawtrx, close, modify, length, create, unlock, check, focus, close_expert_mode, last }; // to gui

    struct cmd_common {
        cmd_common(cmds cmd_ = cmds::unknown): cmd(cmd_){};
        cmds cmd;
        fc_light::variant params;
    };
    struct cmd_base {
        cmd_base(cmds cmd_) : cmd(cmd_) {};
        cmds cmd;
        virtual void operator()(keychain_gui_win&, const fc_light::variant&) const = 0;
        virtual ~cmd_base() {};
    };
    struct cmd_list_singleton
    {
    public:
        static const cmd_list_singleton& instance();
        const std::shared_ptr<cmd_base> operator[](cmds) const;
    private:
        cmd_list_singleton();
        std::vector<std::shared_ptr<cmd_base>> cmd_list;
    };
}

namespace master
{
    enum struct cmds {unknown = 0, ok, cancel, focus, expert_mode, last};

    struct cmd_base{
        cmd_base(cmds c): cmd(c){}
        cmds cmd;
        fc_light::variant params;
    };

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
    template<>
    struct cmd<cmds::focus> : cmd_base {
        cmd(int line_): cmd_base(cmds::focus), focus(line_){params = fc_light::variant(focus);}
        struct params_t {
            params_t(int l): line_edit(l){}
            int line_edit;
        } focus;
    };
    template<>
    struct cmd<cmds::expert_mode> : cmd_base {
        cmd(bool enable_): cmd_base(cmds::expert_mode), mode(enable_){params = fc_light::variant(mode);}
        struct params_t {
            params_t(bool e): enable(e){}
            bool enable;
        } mode;
    };

}

void send(std::string );

FC_LIGHT_REFLECT_ENUM(slave::cmds, (unknown)(rawtrx)(close)(modify)(length)(create)(unlock)(check)(focus)(close_expert_mode)(last))
FC_LIGHT_REFLECT(slave::cmd_common, (cmd)(params))

FC_LIGHT_REFLECT_ENUM(master::cmds, (unknown)(ok)(cancel)(focus)(expert_mode)(last))
FC_LIGHT_REFLECT(master::cmd<master::cmds::ok>::base_t, (cmd))
FC_LIGHT_REFLECT(master::cmd<master::cmds::cancel>::base_t, (cmd))
FC_LIGHT_REFLECT(master::cmd<master::cmds::focus>::params_t, (line_edit))
FC_LIGHT_REFLECT(master::cmd<master::cmds::expert_mode>::params_t, (enable))
FC_LIGHT_REFLECT(master::cmd_base, (cmd)(params))
#endif //KEYCHAINAPP_CMD_H
