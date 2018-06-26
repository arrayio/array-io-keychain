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
#include "widget.hpp"

namespace  gui {
    struct json_err {
        json_err(std::string e): error(e) {};
        std::string error;
    };
    enum struct cmds {unknown = 0, rawtrx, close, modify, length, last };
    struct cmd_common {
        cmd_common(cmds cmd_ = cmds::unknown): cmd(cmd_){};
        cmds cmd;
        fc::variant params;
    };
    struct cmd_base {
        cmd_base(cmds cmd_) : cmd(cmd_) {};
        cmds cmd;
        virtual void operator()(Widget* w, const fc::variant&) const = 0;
        virtual ~cmd_base() {};
    };
    struct cmd_list_singletone
    {
    public:
        static const cmd_list_singletone& instance();
        const std::shared_ptr<cmd_base> operator[](cmds) const;
    private:
        cmd_list_singletone();
        std::vector<std::shared_ptr<cmd_base>> cmd_list;
    };
}

FC_REFLECT_ENUM(gui::cmds, (unknown)(rawtrx)(close)(modify)(length)(last))
FC_REFLECT(gui::cmd_common, (cmd)(params))
FC_REFLECT(gui::json_err, (error))

#endif //KEYCHAINAPP_CMD_H
