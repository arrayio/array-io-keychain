//
// Created by user on 2/26/19.
//
#include <sql_singleton.hpp>

sql_singleton::sql_singleton(){};

sql_singleton::~sql_singleton(){};

sql_singleton& sql_singleton::instance()
{
    static sql_singleton instance = sql_singleton();
    return instance;
}

std::vector<std::string> sql_singleton::select(std::string public_key)
{
 //TODO: need implemantation
 std::vector<std::string> v;
 v.push_back("{\"command\": \"sign_trx\", \"params\": { \"transaction\": \"0100000004bd4f1f39f277201a225d7a197b55ab1412ecd7d6f241f4af3b8b279696b0ae1410000000171600140901ed3d5ce2819be61f98260ebb81436f825eaeffffffff0f17888ae45681a2e806be991b0423a3fec9ce572dec20e107977ad6d18e832f00000000171600144df62b18b614eb0e9678c9156d8cdb41f668d8b6ffffffff870712e48ce6c8d67eb7515b14b97dbe10b06fbe91eb803705feb349a4104d580000000017160014e1d2b15d2bd7175c8bbbf5dd0dc82d481a32bc26ffffffffe5a7c71693f01503f4b241af9d90c52bab6981d63019a32b1f8b0de8fd7f83b40000000017160014a0134fa4fad47c1aa8f9c9d05c0df160dcd4a621ffffffff024d8372010000000017a914a772b3ff281ece609a356f7319fd71217c95c00b870084d717000000001976a914e98d9f78f97003d1d99873c81187657d78dfc46288ac00000000\",  \"blockchain_type\": \"bitcoin\",  \"public_key\": \"512B996635F56C82B9D0387A5CFDDD69B7CDB18868828BA8FF4E2780E521879C117C83098CEF0461077140D84D570F99F3E717299F2BA504562D5B4BE5CDD708\" }}");
 return v;
}

int insert(std::string event)
{
//TODO: need implemantation
    return 0;
}