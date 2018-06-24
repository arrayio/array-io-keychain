//
// Created by user on 23.06.18.
//
#include "cmd.hpp"

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

