//
// Created by user on 3/15/19.
//

#ifndef KEYCHAINAPP_KEYDATA_SINGLETON_HPP
#define KEYCHAINAPP_KEYDATA_SINGLETON_HPP

#include <eth-crypto/crypto/Common.h>

namespace keychain_app {

struct keydata_singleton
{
    static keydata_singleton& instance();
    std::string seed(dev::bytes& );
    void create_masterkey(std::string&, std::string&);
    void create_privatekey();

private:
    keydata_singleton(){}
    ~keydata_singleton(){}
    std::vector<char> pbkdf2(std::string const& _pass);
};

}

#endif //KEYCHAINAPP_KEYDATA_SINGLETON_HPP
