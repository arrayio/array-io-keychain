//
// Created by user on 23.06.18.
//
#include "cmd.hpp"
#include "widget_singleton.h"

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
    struct cmd<cmds::event> : cmd_base {
        cmd() : cmd_base(cmds::event) {};

        struct params {std::string event;};
        using params_t = params;

        virtual ~cmd() {};

        virtual void operator()(keychain_gui_win& w, const fc_light::variant& v) const override {
            try {
                secmod_parser_f event_parser;
                auto cmd = event_parser(v);

                switch (cmd)
                {
                    case sm_cmd::events_te::create_key:
                    {
                        auto event = shared_event::ptr<sm_cmd::events_te::create_key>();
                        event.reset(new  sm_cmd::secmod_event<sm_cmd::events_te::create_key>::params_t (
                                std::move(event_parser.params<sm_cmd::events_te::create_key>())
                        ));
                        auto event_num = shared_event::event_num();
                        event_num = sm_cmd::events_te::create_key;
                        break;
                    }
                    case sm_cmd::events_te::sign_hex:
                    {
                        auto event = shared_event::ptr<sm_cmd::events_te::sign_hex>();
                        event.reset(new  sm_cmd::secmod_event<sm_cmd::events_te::sign_hex>::params_t (
                                std::move(event_parser.params<sm_cmd::events_te::sign_hex>())
                                        ));
                        auto event_num = shared_event::event_num();
                        event_num = sm_cmd::events_te::sign_hex;
                        break;
                    }
                    case sm_cmd::events_te::sign_hash:
                    {
                        auto event = shared_event::ptr<sm_cmd::events_te::sign_hash>();
                        event.reset(new  sm_cmd::secmod_event<sm_cmd::events_te::sign_hash>::params_t (
                                std::move(event_parser.params<sm_cmd::events_te::sign_hash>())
                        ));
                        auto event_num = shared_event::event_num();
                        event_num = sm_cmd::events_te::sign_hash;
                        break;
                    }
                    case sm_cmd::events_te::unlock:
                    {
                        auto event = shared_event::ptr<sm_cmd::events_te::unlock>();
                        event.reset(new  sm_cmd::secmod_event<sm_cmd::events_te::unlock>::params_t (
                                std::move(event_parser.params<sm_cmd::events_te::unlock>())
                        ));
                        auto event_num = shared_event::event_num();
                        event_num = sm_cmd::events_te::unlock;
                        break;
                    }
                }
                w.refresh();
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
                w.close_event = true;
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
            int line_edit;
        };
        using params_t = params;
        virtual void operator()(keychain_gui_win& w, const fc_light::variant& v) const override {
            try {
                auto a = v.as<params_t>();
                w.passentry(a.len, a.line_edit);
            }
            catch (const std::exception &e) {throw std::runtime_error(e.what());}
            catch (const fc_light::exception &e) {throw std::runtime_error(e.what());}
        };
    };

    template<>
    struct cmd<cmds::focus> : cmd_base {
        cmd() : cmd_base(cmds::focus) {};
        virtual ~cmd() {};
        struct params {
            int line;
        };
        using params_t = params;
        virtual void operator()(keychain_gui_win& w, const fc_light::variant& v) const override {
            try {
                auto a = v.as<params_t>();
                w.setFocusByTabKey(a.line);
            }
            catch (const std::exception &e) {throw std::runtime_error(e.what());}
            catch (const fc_light::exception &e) {throw std::runtime_error(e.what());}
        };
    };

    template<>
    struct cmd<cmds::close_expert_mode> : cmd_base {
        cmd() : cmd_base(cmds::close_expert_mode) {};
        virtual ~cmd() {};
        using params_t = void;
        virtual void operator()(keychain_gui_win& w, const fc_light::variant& v) const override {
            try
            {
                w.closeExpertMode();
            }
            catch (const std::exception &e) {throw std::runtime_error(e.what());}
            catch (const fc_light::exception &e) {throw std::runtime_error(e.what());}
        };
    };

    template<>
    struct cmd<cmds::strength> : cmd_base {
        cmd() : cmd_base(cmds::strength) {};
        virtual ~cmd() {};
        struct params {
            strength_te res;
        };
        using params_t = params;
        virtual void operator()(keychain_gui_win& w, const fc_light::variant& v) const override {
            try {
                auto a = v.as<params_t>();
                w.password->checkStrength(a.res);
            }
            catch (const std::exception &e) {throw std::runtime_error(e.what());}
            catch (const fc_light::exception &e) {throw std::runtime_error(e.what());}
        };
    };

    template<>
    struct cmd<cmds::check> : cmd_base {
        cmd() : cmd_base(cmds::check) {};
        virtual ~cmd() {};
        struct params {
            bool res;
        };
        using params_t = params;
        virtual void operator()(keychain_gui_win& w, const fc_light::variant& v) const override {
            try {
                auto a = v.as<params_t>();
                w.password->checkConfirm(a.res);
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

void send(std::string a)
{
    if ( write(STDIN_FILENO, a.c_str(), a.length() ) != a.length() )
        throw std::runtime_error ("error write to pipe");
}



FC_LIGHT_REFLECT(slave::cmd<slave::cmds::event>::params_t, (event))
FC_LIGHT_REFLECT(slave::cmd<slave::cmds::modify>::params_t, (caps)(num)(shift))
FC_LIGHT_REFLECT(slave::cmd<slave::cmds::length>::params_t, (len)(line_edit))
FC_LIGHT_REFLECT(slave::cmd<slave::cmds::check>::params_t, (res))
FC_LIGHT_REFLECT(slave::cmd<slave::cmds::strength>::params_t, (res))
FC_LIGHT_REFLECT(slave::cmd<slave::cmds::focus>::params_t, (line))





