//
// Created by user on 23.06.18.
//
#include "cmd.hpp"

Q_DECLARE_METATYPE(std::string)

namespace hana = boost::hana;
namespace slave
{
    template<cmds cmd_>
    struct cmd : cmd_base {
        cmd() : cmd_base(cmd_) {};
        virtual ~cmd() {};
        virtual void operator()(keychain_gui_win& w, const fc_light::variant& v) const override {
            throw std::runtime_error("operation is not implemented");
        };
        using params_t = void;
    };

    template<>
    struct cmd<cmds::rawtrx> : cmd_base {
        cmd() : cmd_base(cmds::rawtrx) {};

        virtual ~cmd() {};
        struct params {std::string rawtrx;};
        using params_t = params;
        virtual void operator()(keychain_gui_win& w, const fc_light::variant& v) const override {
            try {
                auto a = v.as<params_t>();
                QString  trx(a.rawtrx.c_str());
                Transaction trans(trx);
                w.refresh(trans);
                w.show();
            }
            catch (const std::exception &e) {throw std::runtime_error(e.what());}
            catch (const fc_light::exception &e) {throw std::runtime_error(e.what());}
        };
    };

    template<>
    struct cmd<cmds::close> : cmd_base {
        cmd() : cmd_base(cmds::close) {};
        virtual ~cmd() {};
        using params_t = void;
        virtual void operator()(keychain_gui_win& w, const fc_light::variant& v) const override {
            try
            {
                w.send_msg = false;
                w.close();
            }
            catch (const std::exception &e) {throw std::runtime_error(e.what());}
            catch (const fc_light::exception &e) {throw std::runtime_error(e.what());}
        };
    };

    template<>
    struct cmd<cmds::modify> : cmd_base {
        cmd() : cmd_base(cmds::modify) {};
        virtual ~cmd() {};
        struct params { bool caps, num, shift; };
        using params_t = params;
        virtual void operator()(keychain_gui_win& w, const fc_light::variant& v) const override {
            try {
                auto a = v.as<params_t>();
                //a.caps  ? w->caps->setText("caps: on")   : w->caps->setText("caps: off");
                //a.num   ? w->num->setText("num: on")     : w->num->setText("num: off");
                //a.shift ? w->shift->setText("shift: on")    : w->shift->setText("shift: off");
            }
            catch (const std::exception &e) {throw std::runtime_error(e.what());}
            catch (const fc_light::exception &e) {throw std::runtime_error(e.what());}
        };
    };

    template<>
    struct cmd<cmds::length> : cmd_base {
        cmd() : cmd_base(cmds::length) {};
        virtual ~cmd() {};
        struct params {
            int len;
            int confirm;
        };
        using params_t = params;
        virtual void operator()(keychain_gui_win& w, const fc_light::variant& v) const override {
            try {
                auto a = v.as<params_t>();
                if (a.confirm)
                    w.password->valueConfirm->setText(QString (a.len, '*'));
                else
                    w.password->value->setText(QString (a.len, '*'));
            }
            catch (const std::exception &e) {throw std::runtime_error(e.what());}
            catch (const fc_light::exception &e) {throw std::runtime_error(e.what());}
        };
    };

    template<>
    struct cmd<cmds::create> : cmd_base {
        cmd() : cmd_base(cmds::create) {};
        virtual ~cmd() {};
        struct params {std::string keyname;};
        using params_t = params;
        virtual void operator()(keychain_gui_win& w, const fc_light::variant& v) const override {
            try {
                auto a = v.as<params_t>();
                QString  key(a.keyname.c_str());
                Transaction trans(key);
                trans.setCreatePassword();
                w.refresh(trans);
                w.show();
            }
            catch (const std::exception &e) {throw std::runtime_error(e.what());}
            catch (const fc_light::exception &e) {throw std::runtime_error(e.what());}
        };
    };

    template<>
    struct cmd<cmds::unlock> : cmd_base {
        cmd() : cmd_base(cmds::unlock) {};
        virtual ~cmd() {};
        struct params {
            std::string keyname;
            int unlock_time;
        };
        using params_t = params;
        virtual void operator()(keychain_gui_win& w, const fc_light::variant& v) const override {
            try {
                auto a = v.as<params_t>();
                QString  key(a.keyname.c_str());
                Transaction trans(key);
                trans.setUnlockKey(key, a.unlock_time);
                w.refresh(trans);
                w.show();
            }
            catch (const std::exception &e) {throw std::runtime_error(e.what());}
            catch (const fc_light::exception &e) {throw std::runtime_error(e.what());}
        };
    };

    template<>
    struct cmd<cmds::confirm> : cmd_base {
        cmd() : cmd_base(cmds::confirm) {};
        virtual ~cmd() {};
        struct params {
            bool equal;
        };
        using params_t = params;
        virtual void operator()(keychain_gui_win& w, const fc_light::variant& v) const override {
            try {
                auto a = v.as<params_t>();
                w.password->checkConfirm(a.equal);
            }
            catch (const std::exception &e) {throw std::runtime_error(e.what());}
            catch (const fc_light::exception &e) {throw std::runtime_error(e.what());}
        };
    };

    const cmd_list_singleton& cmd_list_singleton::instance() {
        static const cmd_list_singleton instance;
        return instance;
    }

    cmd_list_singleton::cmd_list_singleton()
    {
        hana::for_each(  hana::make_range(hana::int_c<static_cast<int>(cmds::unknown)>,
                                          hana::int_c<static_cast<int>(cmds::last)>),
                         [&](auto a)
                         {
                             using a_type = decltype(a);
                             constexpr auto v = static_cast<cmds>(a_type::value);
                             cmd_list.push_back( std::shared_ptr<cmd_base>(new cmd<v>()) );
                         });
    };

    const std::shared_ptr<cmd_base> cmd_list_singleton::operator[](cmds cmd_) const {
        size_t a = static_cast<size_t>(cmd_);
        if (a >= cmd_list.size())
            return cmd_list[0];
        return cmd_list[a];
    }
}

FC_LIGHT_REFLECT(slave::cmd<slave::cmds::rawtrx>::params_t, (rawtrx))
FC_LIGHT_REFLECT(slave::cmd<slave::cmds::modify>::params_t, (caps)(num)(shift))
FC_LIGHT_REFLECT(slave::cmd<slave::cmds::length>::params_t, (len)(confirm))
FC_LIGHT_REFLECT(slave::cmd<slave::cmds::create>::params_t, (keyname))
FC_LIGHT_REFLECT(slave::cmd<slave::cmds::unlock>::params_t, (keyname)(unlock_time))
FC_LIGHT_REFLECT(slave::cmd<slave::cmds::confirm>::params_t, (equal))





