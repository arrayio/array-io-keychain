//
// Created by user on 2/26/19.
//
#ifndef KEYCHAINAPP_SQL_SINGLETON_HPP
#define KEYCHAINAPP_SQL_SINGLETON_HPP

#include <vector>
#include <string>
#include <sqlite3.h>

struct sql_singleton
{
    static sql_singleton& instance();
    std::vector<std::string> select(std::string public_key);
    int insert(std::string);
private:
    sql_singleton();
    ~sql_singleton();
};

#endif //KEYCHAINAPP_SQL_SINGLETON_HPP
