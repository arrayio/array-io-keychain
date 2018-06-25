//
// Created by user on 23.06.18.
//
#include "cmd.hpp"

namespace gui
{
    template<cmd_enum cmd_>
    struct cmd : cmd_base {
        cmd() : cmd_base(cmd_) {};
        virtual ~cmd() {};
        virtual void operator()(Widget* w, const fc::variant& v) const override {
            throw std::runtime_error("operation is not implemented");
        };
        using params_t = void;
    };

    template<>
    struct cmd<cmd_rawtrx> : cmd_base {
        cmd() : cmd_base(cmd_rawtrx) {};
        virtual ~cmd() {};
        struct params { std::string rawtrx; };
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
    struct cmd<cmd_close> : cmd_base {
        cmd() : cmd_base(cmd_close) {};
        virtual ~cmd() {};
        using params_t = void;
        virtual void operator()(Widget* w, const fc::variant& v) const override {
            try { w->close(); }
            catch (const std::exception &e) {throw std::runtime_error(e.what());}
            catch (const fc::exception &e) {throw std::runtime_error(e.what());}
        };
    };

    template<>
    struct cmd<cmd_modify> : cmd_base {
        cmd() : cmd_base(cmd_modify) {};
        virtual ~cmd() {};
        struct params { bool caps, num, shift; };
        using params_t = params;
        virtual void operator()(Widget* w, const fc::variant& v) const override {
            try {
                auto a = v.as<params_t>();
            }
            catch (const std::exception &e) {throw std::runtime_error(e.what());}
            catch (const fc::exception &e) {throw std::runtime_error(e.what());}
        };
    };

    template<>
    struct cmd<cmd_length> : cmd_base {
        cmd() : cmd_base(cmd_length) {};
        virtual ~cmd() {};
        struct params { int len; };
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
}

FC_REFLECT(gui::cmd<gui::cmd_rawtrx>::params_t, (rawtrx))
FC_REFLECT(gui::cmd<gui::cmd_modify>::params_t, (caps)(num)(shift))
FC_REFLECT(gui::cmd<gui::cmd_length>::params_t, (len))

using namespace gui;
namespace hana = boost::hana;

const cmd_list_singletone& cmd_list_singletone::instance() {
    static const cmd_list_singletone instance;
    return instance;
}

cmd_list_singletone::cmd_list_singletone()
{
//    cmd_list.reserve(32);
    hana::for_each(  hana::make_range(hana::int_c<cmd_unknown>, hana::int_c<cmd_last>),
                     [&](auto a)
                     {
                         using a_type = decltype(a);
                         constexpr auto v = static_cast<cmd_enum>(a_type::value);
                         cmd_list.push_back( std::shared_ptr<cmd_base>(new cmd<v>()) );
                     });
};

const std::shared_ptr<cmd_base> cmd_list_singletone::operator[](cmd_enum cmd_) const {
    size_t a = static_cast<size_t>(cmd_);
    if (a >= cmd_list.size())
        return cmd_list[0];
    return cmd_list[a];
}

