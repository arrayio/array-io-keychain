//
// Created by user on 23.06.18.
//
#include "cmd.hpp"
namespace hana = boost::hana;
namespace slave
{
    template<cmds cmd_>
    struct cmd : cmd_base {
        cmd() : cmd_base(cmd_) {};
        virtual ~cmd() {};
        virtual void operator()(polling* p, const fc::variant& v) const override {
            throw std::runtime_error("operation is not implemented");
        };
        using params_t = void;
    };
    template<>
    struct cmd<cmds::ok> : cmd_base {
        cmd() : cmd_base(cmds::ok) {};
        virtual ~cmd() {};
        using params_t = void;
        virtual void operator()(polling* p, const fc::variant& v) const override {
            try { p->closeEvent=true;}
            catch (const std::exception &e) {throw std::runtime_error(e.what());}
            catch (const fc::exception &e) {throw std::runtime_error(e.what());}
        };
    };
    template<>
    struct cmd<cmds::cancel> : cmd_base {
        cmd() : cmd_base(cmds::cancel) {};
        virtual ~cmd() {};
        using params_t = void;
        virtual void operator()(polling* p, const fc::variant& v) const override {
            try { p->closeEvent=true; p->passClearOnExit=true;}
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


