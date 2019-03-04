//
// Created by user on 2/26/19.
//
#include <sql_singleton.hpp>
#include "keychain_commands.hpp"
#include <boost/filesystem.hpp>
#define SQL_DB_DEFAULT_PATH KEY_DEFAULT_PATH "/sql"

namespace bfs = boost::filesystem;

sql_singleton::sql_singleton()
{
#if defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
  auto sql_dir = bfs::path(getenv("HOME"));
  sql_dir += bfs::path("/" SQL_DB_DEFAULT_PATH);
#else
  bfs::path sql_dir(SQL_DB_DEFAULT_PATH);
#endif

    bfs::path key_dir(sql_dir);
    if(!bfs::exists(sql_dir))
    {
        auto res = bfs::create_directories(sql_dir);
        if(res == false)
            FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception,
                                     "Can not create sql directory, path = ${directory}", ("directory", sql_dir.string()));
    }
    sqlite3_stmt * stmt;
        const char * statement =   "create table if not exists log (public_key text not null, "
                                   "trx text not null)";
    sql_dir += "/data.db";
    if  (sqlite3_open_v2(sql_dir.c_str(), &db, SQLITE_OPEN_FULLMUTEX|SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE, 0)
            != SQLITE_OK )
        FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "sqlite3_open");

    auto res = sqlite3_prepare_v2(db, statement, -1, &stmt, NULL);
    if  ( res != SQLITE_OK )
        FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "sqlite3_prepare_v2");

    sqlite3_step(stmt);
    if  (sqlite3_finalize(stmt) != SQLITE_OK )
        FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "sqlite3_finalize");
};

sql_singleton::~sql_singleton()
{
    sqlite3_close_v2(db);
};

sql_singleton& sql_singleton::instance()
{
    static sql_singleton instance = sql_singleton();
    return instance;
}

std::vector<std::string> sql_singleton::select(std::string& public_key)
{
    sqlite3_stmt * stmt;
    std::vector<std::string> set;
    const char * statement =   "select trx from log where public_key=?";

    auto res = sqlite3_prepare_v2(db, statement, -1, &stmt, NULL);
    if  ( res != SQLITE_OK )
        FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "sqlite3_prepare_v2");

    sqlite3_bind_text(stmt, 1, public_key.c_str(), -1, 0);

    while(true)
    {
        res = sqlite3_step(stmt);
        if (res == SQLITE_ROW)
          set.push_back(std::string ((const char *) sqlite3_column_text(stmt, 0)) );
        else break;
    }

    if  (sqlite3_finalize(stmt) != SQLITE_OK )
        FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "sqlite3_finalize");
    return set;
}

int sql_singleton::insert(std::string& key, std::string& val)
{
    sqlite3_stmt * stmt;
    std::string statement = "insert into log (public_key, trx) values('"+ key + "', '"+val+"')";

    auto res = sqlite3_prepare_v2(db, statement.c_str(), -1, &stmt, NULL);
    if  ( res != SQLITE_OK )
        FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "sqlite3_prepare_v2");

    res = sqlite3_step(stmt);
    if (res != SQLITE_DONE)
        FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "sqlite3_step");

    if  (sqlite3_finalize(stmt) != SQLITE_OK )
        FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "sqlite3_finalize");

    return 0;
}
