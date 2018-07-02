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
        virtual void operator()(Widget* w, const fc::variant& v) const override {
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
        virtual void operator()(Widget * w, const fc::variant& v) const override {
            try {
                auto a = v.as<params_t>();
                w->pte->setText(QString(a.rawtrx.c_str()));
            }
            catch (const std::exception &e) {throw std::runtime_error(e.what());}
            catch (const fc::exception &e) {throw std::runtime_error(e.what());}
        };
    };

    template<>
    struct cmd<cmds::close> : cmd_base {
        cmd() : cmd_base(cmds::close) {};
        virtual ~cmd() {};
        using params_t = void;
        virtual void operator()(Widget* w, const fc::variant& v) const override {
            try { w->close(); }
            catch (const std::exception &e) {throw std::runtime_error(e.what());}
            catch (const fc::exception &e) {throw std::runtime_error(e.what());}
        };
    };

    template<>
    struct cmd<cmds::modify> : cmd_base {
        cmd() : cmd_base(cmds::modify) {};
        virtual ~cmd() {};
        struct params { bool caps, num, shift; };
        using params_t = params;
        virtual void operator()(Widget* w, const fc::variant& v) const override {
            try {
                auto a = v.as<params_t>();
                a.caps  ? w->caps->setText("caps: on")   : w->caps->setText("caps: off");
                a.num   ? w->num->setText("num: on")     : w->num->setText("num: off");
                a.shift ? w->shift->setText("shift: on")    : w->shift->setText("shift: off");
            }
            catch (const std::exception &e) {throw std::runtime_error(e.what());}
            catch (const fc::exception &e) {throw std::runtime_error(e.what());}
        };
    };

    template<>
    struct cmd<cmds::length> : cmd_base {
        cmd() : cmd_base(cmds::length) {};
        virtual ~cmd() {};
        struct params {int len;};
        using params_t = params;
        virtual void operator()(Widget* w, const fc::variant& v) const override {
            try {
                auto a = v.as<params_t>();
                w->ple->setText(QString (a.len, '*'));
            }
            catch (const std::exception &e) {throw std::runtime_error(e.what());}
            catch (const fc::exception &e) {throw std::runtime_error(e.what());}
        };
    };


    const cmd_list_singletone& cmd_list_singletone::instance() {
        static const cmd_list_singletone instance;
        return instance;
    }

    cmd_list_singletone::cmd_list_singletone()
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

    const std::shared_ptr<cmd_base> cmd_list_singletone::operator[](cmds cmd_) const {
        size_t a = static_cast<size_t>(cmd_);
        if (a >= cmd_list.size())
            return cmd_list[0];
        return cmd_list[a];
    }
}

FC_REFLECT(slave::cmd<slave::cmds::rawtrx>::params_t, (rawtrx))
FC_REFLECT(slave::cmd<slave::cmds::modify>::params_t, (caps)(num)(shift))
FC_REFLECT(slave::cmd<slave::cmds::length>::params_t, (len))




