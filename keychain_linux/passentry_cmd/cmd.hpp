//
// Created by user on 23.06.18.
//
#ifndef KEYCHAINAPP_CMD_H
#define KEYCHAINAPP_CMD_H
#include <fc_light/reflect/reflect.hpp>
#include <fc_light/reflect/variant.hpp>
#include <fc_light/variant.hpp>
#include <fc_light/io/json.hpp>
#include <iostream>
#include <fc_light/exception/exception.hpp>
#include <fc_light/exception/exception.hpp>

#undef Bool //NOTE: Xlib contains #define Bool, that causes error compilation hana
#undef None //NOTE: Xlib contains #define None, that causes error compilation hana
#include <boost/hana.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/for_each.hpp>
#include <boost/hana/size.hpp>
#include "polling.hpp"
#include "../passentry_gui/include/password_strength.h"

namespace  slave {
    enum struct cmds {unknown = 0, ok, cancel, focus, expert_mode, last}; //from gui

    struct cmd_common {
        cmd_common(cmds cmd_ = cmds::unknown): cmd(cmd_){};
        cmds cmd;
        fc_light::variant params;
    };

    struct cmd_base {
        cmd_base(cmds cmd_) : cmd(cmd_) {};
        cmds cmd;
        virtual void operator()(polling*, const fc_light::variant&) const = 0;
        virtual ~cmd_base() {};
    };

    template<cmds cmd_>
    struct cmd : cmd_base {
        cmd() : cmd_base(cmd_) {};
        virtual ~cmd() {};
        virtual void operator()(polling* p, const fc_light::variant& v) const override {
            throw std::runtime_error("operation is not implemented");
        };
        using params_t = void;
    };
    template<>
    struct cmd<cmds::ok> : cmd_base {
        cmd() : cmd_base(cmds::ok) {};
        virtual ~cmd() {};
        using params_t = void;
        virtual void operator()(polling* p, const fc_light::variant& v) const override {
            try { p->OkButtonPressEvent=true;}  
            catch (const std::exception &e) {throw std::runtime_error(e.what());}
            catch (const fc_light::exception &e) {throw std::runtime_error(e.what());}
        };
    };
    template<>
    struct cmd<cmds::cancel> : cmd_base {
        cmd() : cmd_base(cmds::cancel) {};
        virtual ~cmd() {};
        using params_t = void;
        virtual void operator()(polling* p, const fc_light::variant& v) const override {
            try { p->CancelButtonPressEvent=true; }
            catch (const std::exception &e) {throw std::runtime_error(e.what());}
            catch (const fc_light::exception &e) {throw std::runtime_error(e.what());}
        };
    };
    template<>
    struct cmd<cmds::focus> : cmd_base {
        cmd() : cmd_base(cmds::focus) {};
        virtual ~cmd() {};
        struct params {int line_edit;};
        using params_t = params;
        virtual void operator()(polling* p, const fc_light::variant& v) const override {
            try {
                auto a = v.as<params>();
                p->line_edit = a.line_edit;
                p->focusEvent = true;
            }
            catch (const std::exception &e) {throw std::runtime_error(e.what());}
            catch (const fc_light::exception &e) {throw std::runtime_error(e.what());}
        };
    };

    template<>
    struct cmd<cmds::expert_mode> : cmd_base {
        cmd() : cmd_base(cmds::expert_mode) {};
        virtual ~cmd() {};
        struct params {bool enable;};
        using params_t = params;
        virtual void operator()(polling* p, const fc_light::variant& v) const override {
            try {
                auto a = v.as<params>();
                p->expert_mode = a.enable;
            }
            catch (const std::exception &e) {throw std::runtime_error(e.what());}
            catch (const fc_light::exception &e) {throw std::runtime_error(e.what());}
        };
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

namespace  master {
    enum struct cmds {unknown = 0, event, close, modify, length, check, focus, close_expert_mode, strength, last};

    struct cmd_base {
        cmd_base(): cmd(cmds::unknown){};
        cmds cmd;
        fc_light::variant params;
    };
    template<cmds cmd_>
    struct cmd : cmd_base {};

    template<>
    struct cmd<cmds::event> : cmd_base{
        cmd(std::string event_): cmd_base(), event(event_){
            cmd_base::cmd = cmds::event;
            params = fc_light::variant(event);
        };
        struct params_t { params_t(std::string s):event(s){}
            fc_light::variant event;
        } event;
    };

    template<>
    struct cmd<cmds::close> {
        cmd():base(){}
        struct params_t {params_t():cmd(cmds::close){} cmds cmd;} base;
    };

    template<>
    struct cmd<cmds::modify> : cmd_base {
        cmd(bool caps_, bool num_, bool shift_): cmd_base(), modify(caps_, num_, shift_){
            cmd_base::cmd = cmds::modify;
            params = fc_light::variant(modify);
        };
        struct params_t { params_t(bool caps_, bool num_, bool shift_):caps(caps_), num(num_), shift(shift_){}
            fc_light::variant caps, num, shift;
        }  modify;
    };

    template<>
    struct cmd<cmds::length> : cmd_base{
        cmd(int l, int le): cmd_base(), length(l, le){
            cmd_base::cmd = cmds::length;
            params = fc_light::variant(length);
        };
        struct params_t { params_t(int l, int le):len(l), line_edit(le){}
            int len;
            int line_edit;
        } length;
    };

    template<>
    struct cmd<cmds::check> : cmd_base{
        cmd(bool res): cmd_base(), check_param(res){
            cmd_base::cmd = cmds::check;
            params = fc_light::variant(check_param);
        };
        struct params_t {
            params_t(bool res_):res(res_) {}
            bool res;
        } check_param;
    };

    template<>
    struct cmd<cmds::focus> : cmd_base{
        cmd(int line): cmd_base(), focus_param(line){
            cmd_base::cmd = cmds::focus;
            params = fc_light::variant(focus_param);
        };
        struct params_t {
            params_t(int l): line(l) {}
            int line;
        } focus_param;
    };

    template<>
    struct cmd<cmds::close_expert_mode> {
        cmd():base(){}
        struct params_t {params_t():cmd(cmds::close_expert_mode){} cmds cmd;} base;
    };

    template<>
    struct cmd<cmds::strength> : cmd_base{
        cmd(strength_te res): cmd_base(), strength_param(res){
            cmd_base::cmd = cmds::strength;
            params = fc_light::variant(strength_param);
        };
        struct params_t {
            params_t(strength_te res_):res(res_) {}
            strength_te  res;
        } strength_param;
    };
}

FC_LIGHT_REFLECT_ENUM(master::cmds, (unknown)(event)(close)(modify)(length)(check)(focus)(close_expert_mode)(strength)(last))
FC_LIGHT_REFLECT(master::cmd_base, (cmd)(params))
FC_LIGHT_REFLECT(master::cmd<master::cmds::event>::params_t, (event))
FC_LIGHT_REFLECT(master::cmd<master::cmds::close>::params_t, (cmd))
FC_LIGHT_REFLECT(master::cmd<master::cmds::modify>::params_t, (caps)(num)(shift))
FC_LIGHT_REFLECT(master::cmd<master::cmds::length>::params_t, (len)(line_edit))
FC_LIGHT_REFLECT(master::cmd<master::cmds::check>::params_t, (res))
FC_LIGHT_REFLECT(master::cmd<master::cmds::focus>::params_t, (line))
FC_LIGHT_REFLECT(master::cmd<master::cmds::strength>::params_t, (res))
FC_LIGHT_REFLECT(master::cmd<master::cmds::close_expert_mode>::params_t, (cmd))

FC_LIGHT_REFLECT_ENUM(slave::cmds, (unknown)(ok)(cancel)(focus)(expert_mode)(last))
FC_LIGHT_REFLECT(slave::cmd_common, (cmd)(params))
FC_LIGHT_REFLECT(slave::cmd<slave::cmds::focus>::params_t, (line_edit))
FC_LIGHT_REFLECT(slave::cmd<slave::cmds::expert_mode>::params_t, (enable))

FC_LIGHT_REFLECT_ENUM(strength_te, (unknown)(weak)(middle)(strong)(last))

#endif //KEYCHAINAPP_CMD_H
