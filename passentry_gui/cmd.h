//
// Created by user on 23.06.18.
//
#ifndef KEYCHAINAPP_CMD_H
#define KEYCHAINAPP_CMD_H
#include <fc/reflect/reflect.hpp>
//#include <fc/variant.hpp>
#include <graphene/chain/protocol/protocol.hpp>
//#include <graphene/chain/protocol/transaction.hpp>
#include <fc/io/json.hpp>

#include <iostream>
#include <vector>
//#include <boost/hana/for_each.hpp>
//#include <boost/hana/size.hpp>
//#include <boost/hana.hpp>
//#include <boost/hana/range.hpp>




//#include <type_traits>
//#include <string>

namespace  gui {
    enum cmd_enum {
        cmd_unknown = 0, cmd_rawtrx, cmd_close, cmd_modify, cmd_length, cmd_last
    };

    struct json_err {
        json_err(int i, std::string e) : id(i), error(e) {};
        int id;
        std::string error;
    };

    struct cmd_common {
        cmd_common(cmd_enum cmd_ = cmd_unknown, int id_ = 0) : cmd(cmd_), id(id_) {};
        cmd_enum cmd;
        int id;
        fc::variant params;
    };

    struct cmd_base {
        cmd_base(cmd_enum cmd_) : cmd(cmd_) {};
        cmd_enum cmd;
        virtual std::string operator()(const fc::variant&, int) const = 0;
        virtual ~cmd_base() {};
    };


    template<cmd_enum cmd_>
    struct cmd : cmd_base {
        cmd() : cmd_base(cmd_) {};
        virtual ~cmd() {};
        virtual std::string operator()(const fc::variant& v, int id) const {
            return fc::json::to_pretty_string(fc::variant(json_err(id, "operation is not implemented")));
        };
        using params_t = void;
    };

    template<>
    struct cmd<cmd_rawtrx> : cmd_base {
        cmd() : cmd_base(cmd_rawtrx) {};

        virtual ~cmd() {};
        struct params {
            std::string cmd;
            std::string value;
        };
        using params_t = params;

        virtual std::string operator()(const fc::variant& v, int id) const override {
            try {
                auto a = v.as<params_t>();
                return fc::json::to_pretty_string(a.value);
            }
            catch (std::exception &e) {
                std::cerr << fc::json::to_pretty_string(fc::variant(json_err(id, e.what()))) << std::endl;
                return fc::json::to_pretty_string(fc::variant(json_err(id, e.what())));
            }
        };
    };

    template<>
    struct cmd<cmd_close> : cmd_base {
        cmd() : cmd_base(cmd_close) {};

        virtual ~cmd() {};
        struct params {
            std::string cmd;
            std::string value;
        };
        using params_t = params;

        virtual std::string operator()(const fc::variant& v, int id) const override {
            try {
                auto a = v.as<params_t>();
                return fc::json::to_pretty_string(a.value);
            }
            catch (std::exception &e) {
                std::cerr << fc::json::to_pretty_string(fc::variant(json_err(id, e.what()))) << std::endl;
                return fc::json::to_pretty_string(fc::variant(json_err(id, e.what())));
            }
        };
    };

    template<>
    struct cmd<cmd_modify> : cmd_base {
        cmd() : cmd_base(cmd_modify) {};

        virtual ~cmd() {};
        struct params {
            std::string cmd;
            bool caps, num, shift;
        };
        using params_t = params;

        virtual std::string operator()(const fc::variant& v, int id) const override {
            try {
                auto a = v.as<params_t>();
                return fc::json::to_pretty_string(a.cmd);
            }
            catch (std::exception &e) {
                std::cerr << fc::json::to_pretty_string(fc::variant(json_err(id, e.what()))) << std::endl;
                return fc::json::to_pretty_string(fc::variant(json_err(id, e.what())));
            }
        };
    };

    template<>
    struct cmd<cmd_length> : cmd_base {
        cmd() : cmd_base(cmd_length) {};

        virtual ~cmd() {};
        struct params {
            std::string cmd;
            int len;
        };
        using params_t = params;

        virtual std::string operator()(const fc::variant& v, int id) const override {
            try {
                auto a = v.as<params_t>();
                return fc::json::to_pretty_string(std::to_string(a.len));
            }
            catch (std::exception &e) {
                std::cerr << fc::json::to_pretty_string(fc::variant(json_err(id, e.what()))) << std::endl;
                return fc::json::to_pretty_string(fc::variant(json_err(id, e.what())));
            }
        };
    };
/*
    struct cmd_list_singletone
    {
    public:
        static const cmd_list_singletone& instance();
        const std::shared_ptr<cmd_base> operator[](cmd_enum) const;
    private:
        cmd_list_singletone();
        std::vector<std::shared_ptr<cmd_base>> cmd_list;
    };
*/
    //constexpr auto cmd_range =  hana::make_range(hana::int_c<cmd_unknown>, hana::int_c<cmd_last>);
}

FC_REFLECT_ENUM(gui::cmd_enum, (cmd_unknown)(cmd_rawtrx)(cmd_close)(cmd_modify)(cmd_length)(cmd_last))
FC_REFLECT(gui::cmd<gui::cmd_rawtrx>::params_t, (cmd)(value))
FC_REFLECT(gui::cmd<gui::cmd_close>::params_t, (cmd)(value))
FC_REFLECT(gui::cmd<gui::cmd_modify>::params_t, (cmd)(caps)(num)(shift))
FC_REFLECT(gui::cmd<gui::cmd_length>::params_t, (cmd)(len))
FC_REFLECT(gui::json_err, (id)(error))

#endif //KEYCHAINAPP_CMD_H
