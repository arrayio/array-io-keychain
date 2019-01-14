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

namespace  slave {
    enum struct cmds {unknown = 0, ok, cancel, focus, last}; //from gui

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
            try { p->closeEvent=true;}
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
            try { p->closeEvent=true; p->passClearOnExit=true;}
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
    enum struct cmds {unknown = 0, rawtrx, close, modify, length, create, unlock, check, focus, last};

    struct cmd_base {
        cmd_base(): cmd(cmds::unknown){};
        cmds cmd;
        fc_light::variant params;
    };
    template<cmds cmd_>
    struct cmd : cmd_base {};

    template<>
    struct cmd<cmds::rawtrx> : cmd_base{
        cmd(std::string rawtrx_): cmd_base(), rawtrx(rawtrx_){
            cmd_base::cmd = cmds::rawtrx;
            params = fc_light::variant(rawtrx);
        };
        struct params_t { params_t(std::string s):rawtrx(s){}
            fc_light::variant rawtrx;
        } rawtrx;
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
    struct cmd<cmds::create> : cmd_base{
        cmd(std::string name): cmd_base(), key_name(name){
            cmd_base::cmd = cmds::create;
            params = fc_light::variant(key_name);
        };
        struct params_t { params_t(std::string s):keyname(s){}
            std::string keyname;
        } key_name;
    };

    template<>
    struct cmd<cmds::unlock> : cmd_base{
        cmd(std::string name, int time): cmd_base(), unlock_param(name, time){
            cmd_base::cmd = cmds::unlock;
            params = fc_light::variant(unlock_param);
        };
        struct params_t {
            params_t(std::string s, int t):keyname(s), unlock_time(t) {}
            std::string keyname;
            int unlock_time;
        } unlock_param;
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

}

FC_LIGHT_REFLECT_ENUM(master::cmds, (unknown)(rawtrx)(close)(modify)(length)(unlock)(check)(focus)(last))
FC_LIGHT_REFLECT(master::cmd_base, (cmd)(params))
FC_LIGHT_REFLECT(master::cmd<master::cmds::rawtrx>::params_t, (rawtrx))
FC_LIGHT_REFLECT(master::cmd<master::cmds::close>::params_t, (cmd))
FC_LIGHT_REFLECT(master::cmd<master::cmds::modify>::params_t, (caps)(num)(shift))
FC_LIGHT_REFLECT(master::cmd<master::cmds::length>::params_t, (len)(line_edit))
FC_LIGHT_REFLECT(master::cmd<master::cmds::create>::params_t, (keyname))
FC_LIGHT_REFLECT(master::cmd<master::cmds::unlock>::params_t, (keyname)(unlock_time))
FC_LIGHT_REFLECT(master::cmd<master::cmds::check>::params_t, (res))
FC_LIGHT_REFLECT(master::cmd<master::cmds::focus>::params_t, (line))

FC_LIGHT_REFLECT_ENUM(slave::cmds, (unknown)(ok)(cancel)(focus)(last))
FC_LIGHT_REFLECT(slave::cmd_common, (cmd)(params))
FC_LIGHT_REFLECT(slave::cmd<slave::cmds::focus>::params_t, (line_edit))

#endif //KEYCHAINAPP_CMD_H
