//
// Created by user on 2/26/19.
//
#include <sql_singleton.hpp>
#include <boost/filesystem.hpp>
#define SQL_DB_DEFAULT_PATH KEY_DEFAULT_PATH "/sql"

namespace bfs = boost::filesystem;

using namespace keychain_app;

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
    sql_dir += "/data.db";
    if  (sqlite3_open_v2(sql_dir.c_str(), &db, SQLITE_OPEN_FULLMUTEX|SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE, 0)
            != SQLITE_OK )
        FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "sqlite3_open");

    const char * signlog =  "create table if not exists signlog (public_key text not null, "
                                "trx text not null, sign_time text not null, blockchain_type text not null)";
    const char * keypath =  "create table if not exists keypath (keyname text not null, root text not null, "
                            "purpose integer not null, coin_type integer not null, "
                            "account integer not null, change integer not null, "
                            "address_index integer not null,"
                            "UNIQUE(keyname, root, purpose, coin_type, account, change, address_index))";
    execute(signlog);
    execute(keypath);
};


void sql_singleton::execute(const char * statement)
{
    sqlite3_stmt * stmt;

    if  ( sqlite3_prepare_v2(db, statement, -1, &stmt, NULL) != SQLITE_OK )
        FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "sqlite3_prepare_v2");

    if (sqlite3_step(stmt) != SQLITE_DONE)
        FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "sqlite3_step");

    if  (sqlite3_finalize(stmt) != SQLITE_OK )
        FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "sqlite3_finalize");
}

sql_singleton::~sql_singleton()
{
    sqlite3_close_v2(db);
};

sql_singleton& sql_singleton::instance()
{
    static sql_singleton instance = sql_singleton();
    return instance;
}

const std::vector<keyfile_format::log_record> sql_singleton::select_log(const dev::Public& pkey)
{
    sqlite3_stmt * stmt;
    std::vector<keyfile_format::log_record> records;
    const char * statement =   "select trx, sign_time, blockchain_type from signlog where public_key=?";

    if  ( sqlite3_prepare_v2(db, statement, -1, &stmt, NULL) != SQLITE_OK )
        FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "sqlite3_prepare_v2");

    std::string hex = pkey.hex();
    sqlite3_bind_text(stmt, 1, hex.c_str(), -1, 0);

    while(true)
    {
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            std::string trx((const char *) sqlite3_column_text(stmt, 0));
            std::string time((const char *) sqlite3_column_text(stmt, 1));
            std::string chain((const char *) sqlite3_column_text(stmt, 2));

            keyfile_format::log_record rec;
            rec.transaction.resize(trx.length());
            auto len = from_hex(trx, rec.transaction.data(), rec.transaction.size() );
            rec.transaction.resize(len);
            try {
                rec.sign_time = fc_light::variant(time).as<fc_light::time_point>();
                rec.blockchain_type = fc_light::variant(chain).as<blockchain_te>();
            }
            catch (const std::exception &e) {throw std::runtime_error(e.what());}
            catch (const fc_light::exception &e) {throw std::runtime_error(e.what());}

            records.push_back(rec);
        }
        else break;
    }

    if  (sqlite3_finalize(stmt) != SQLITE_OK )
        FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "sqlite3_finalize");

    return  records;
}


const std::vector<keydata::backup_t> sql_singleton::select_path()
{
    sqlite3_stmt * stmt;
    std::vector<keydata::backup_t> records;
    const char * statement =   "select keyname, root, purpose, coin_type, account, change, address_index from keypath";

    if  ( sqlite3_prepare_v2(db, statement, -1, &stmt, NULL) != SQLITE_OK )
        FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "sqlite3_prepare_v2");

    while(true)
    {
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            keydata::backup_t backup;
            keydata::path_levels_t path;

            backup.keyname  = std::string((const char *) sqlite3_column_text(stmt, 0));
            path.root = std::string((const char *) sqlite3_column_text(stmt, 1));
            path.purpose =  std::stoi(std::string((const char *) sqlite3_column_text(stmt, 2)));
            path.coin_type =  std::stoi(std::string((const char *) sqlite3_column_text(stmt, 3)));
            path.account =  std::stoi(std::string((const char *) sqlite3_column_text(stmt, 4)));
            path.change =  std::stoi(std::string((const char *) sqlite3_column_text(stmt, 5)));
            path.address_index =  std::stoi(std::string((const char *) sqlite3_column_text(stmt,6)));
            backup.path = fc_light::variant(path);

            records.push_back(backup);
        }
        else break;
    }

    if  (sqlite3_finalize(stmt) != SQLITE_OK )
        FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "sqlite3_finalize");

    return  records;
}


int sql_singleton::insert_log(const dev::Public& pkey, const keyfile_format::log_record& record)
{
    sqlite3_stmt * stmt;
    fc_light::variant vtime(record.sign_time);
    auto time  = vtime.as_string();
    fc_light::variant vtype(record.blockchain_type);
    auto type  = vtype.as_string();

    std::string statement = "insert into signlog (public_key, trx, sign_time, blockchain_type)"
                            " values('"+pkey.hex()+"', '"+
                            to_hex(record.transaction.data(), record.transaction.size())+"', '"+
                            time+"', '"+type+ "')";
    execute(statement.c_str());

    return 0;
}


int sql_singleton::insert_path(const keydata::backup_t&  backup )
{
    sqlite3_stmt * stmt;
    keydata::path_levels_t path;
    try {
        path = backup.path.as<keydata::path_levels_t>();
    }
    catch (const std::exception &e) {throw std::runtime_error(e.what());}
    catch (const fc_light::exception &e) {throw std::runtime_error(e.what());}

    std::string statement = "insert or replace into keypath (keyname, root, purpose, coin_type, account, change, address_index )"
                            " values('"+backup.keyname+"', '"+
                            path.root + "', '" +
                            std::to_string(path.purpose)+"', '" +
                            std::to_string(path.coin_type)+"', '"+
                            std::to_string(path.account )+ "', '" +
                            std::to_string(path.change)+"', '" +
                            std::to_string(path.address_index)+ "')";
    execute(statement.c_str());
    return 0;
}