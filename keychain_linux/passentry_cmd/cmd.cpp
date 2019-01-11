//
// Created by user on 23.06.18.
//
#include "cmd.hpp"
namespace hana = boost::hana;
namespace slave
{
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


