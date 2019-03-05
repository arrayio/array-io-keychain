//
// Created by user on 2/26/19.
//
#include <sql_singleton.hpp>
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
        const char * statement =   "create table if not exists signlog (public_key text not null, "
                                   "trx text not null, sign_time text not null, blockchain_type text not null)";
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

const std::vector<keychain_app::keyfile_format::log_record> sql_singleton::select_log(const dev::Public& pkey)
{
    sqlite3_stmt * stmt;
    std::vector<keychain_app::keyfile_format::log_record> records;
    const char * statement =   "select trx, sign_time, blockchain_type from signlog where public_key=?";

    auto res = sqlite3_prepare_v2(db, statement, -1, &stmt, NULL);
    if  ( res != SQLITE_OK )
        FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "sqlite3_prepare_v2");

    std::string hex = pkey.hex();
    sqlite3_bind_text(stmt, 1, hex.c_str(), -1, 0);

    while(true)
    {
        res = sqlite3_step(stmt);
        if (res == SQLITE_ROW)
        {
            std::string trx((const char *) sqlite3_column_text(stmt, 0));
            std::string time((const char *) sqlite3_column_text(stmt, 1));
            std::string chain((const char *) sqlite3_column_text(stmt, 2));

            keychain_app::keyfile_format::log_record rec;
            rec.transaction.resize(trx.length());
            auto len = keychain_app::from_hex(trx, rec.transaction.data(), rec.transaction.size() );
            rec.transaction.resize(len);
            rec.sign_time = fc_light::variant(time).as<fc_light::time_point>();
            rec.blockchain_type = fc_light::variant(chain).as<keychain_app::blockchain_te>();
            records.push_back(rec);
        }
        else break;
    }

    if  (sqlite3_finalize(stmt) != SQLITE_OK )
        FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "sqlite3_finalize");

    return  records;
}

int sql_singleton::insert_log(const dev::Public& pkey, const keychain_app::keyfile_format::log_record& record)
{
    sqlite3_stmt * stmt;
    fc_light::variant vtime(record.sign_time);
    auto time  = vtime.as_string();
    fc_light::variant vtype(record.blockchain_type);
    auto type  = vtype.as_string();

    std::string statement = "insert into signlog (public_key, trx, sign_time, blockchain_type)"
                            " values('"+pkey.hex()+"', '"+
                            keychain_app::to_hex(record.transaction.data(), record.transaction.size())+"', '"+
                            time+"', '"+type+ "')";

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