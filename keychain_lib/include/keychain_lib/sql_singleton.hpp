//
// Created by user on 2/26/19.
//
#ifndef KEYCHAINAPP_SQL_SINGLETON_HPP
#define KEYCHAINAPP_SQL_SINGLETON_HPP

#include <sqlite3.h>
#include "keychain_commands.hpp"
#include "keyfile_singleton.hpp"

struct sql_singleton
{
    static sql_singleton& instance();
    const std::vector<keychain_app::keyfile_format::log_record> select_log(const dev::Public& );
    int insert_log(const dev::Public&, const keychain_app::keyfile_format::log_record& );

private:
    sql_singleton();
    ~sql_singleton();

    sqlite3 * db;
};

#endif //KEYCHAINAPP_SQL_SINGLETON_HPP
