//
// Created by user on 23.06.18.
//
#ifndef KEYCHAINAPP_CMD_H
#define KEYCHAINAPP_CMD_H
#include <fc/reflect/reflect.hpp>
#include <graphene/chain/protocol/protocol.hpp>
#include <fc/io/json.hpp>
#include <iostream>
#include <vector>
#include <boost/hana.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/for_each.hpp>
#include <boost/hana/size.hpp>
#include <boost/hana.hpp>
#include "widget.hpp"

namespace  gui {
    struct json_err {
        json_err(std::string e): error(e) {};
        std::string error;
    };

    enum cmd_enum {
        cmd_unknown = 0, cmd_rawtrx, cmd_close, cmd_modify, cmd_length, cmd_last
    };
    struct cmd_common {
        cmd_common(cmd_enum cmd_ = cmd_unknown): cmd(cmd_){};
        cmd_enum cmd;
        fc::variant params;
    };
    struct cmd_base {
        cmd_base(cmd_enum cmd_) : cmd(cmd_) {};
        cmd_enum cmd;
        virtual void operator()(Widget* w, const fc::variant&) const = 0;
        virtual ~cmd_base() {};
    };
    struct cmd_list_singletone
    {
    public:
        static const cmd_list_singletone& instance();
        const std::shared_ptr<cmd_base> operator[](cmd_enum) const;
    private:
        cmd_list_singletone();
        std::vector<std::shared_ptr<cmd_base>> cmd_list;
    };
}

FC_REFLECT_ENUM(gui::cmd_enum, (cmd_unknown)(cmd_rawtrx)(cmd_close)(cmd_modify)(cmd_length)(cmd_last))
FC_REFLECT(gui::cmd_common, (cmd)(params))
FC_REFLECT(gui::json_err, (error))

#endif //KEYCHAINAPP_CMD_H
