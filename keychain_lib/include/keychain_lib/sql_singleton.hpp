//
// Created by user on 2/26/19.
//
#ifndef KEYCHAINAPP_SQL_SINGLETON_HPP
#define KEYCHAINAPP_SQL_SINGLETON_HPP

#include <sqlite3.h>
#include "keychain_commands.hpp"
#include "keyfile_singleton.hpp"
#include "keydata_singleton.hpp"

namespace keychain_app
{

struct sql_singleton
{
    static sql_singleton& instance();
    const std::vector<keyfile_format::log_record> select_log(const dev::Public& );
    int insert_log(const dev::Public&, const keyfile_format::log_record& );
    int insert_path(const keydata::backup_t&);
    const std::vector<keydata::backup_t> select_path();

private:
    sql_singleton();
    ~sql_singleton();
    void execute(const char *);
    sqlite3 * db;
};

}

#endif //KEYCHAINAPP_SQL_SINGLETON_HPP
