
#ifndef KEYCHAINAPP_SEC_MOD_LINUX_HPP
#define KEYCHAINAPP_SEC_MOD_LINUX_HPP

#include <keychain_lib/keychain.hpp>

namespace keychain_app
{

    class sec_mod_linux: public secure_dlg_mod_base
    {
    public:
        sec_mod_linux();
        virtual ~sec_mod_linux();
        virtual std::string exec_cmd(const std::string& json_cmd) const override;
    };

}

#endif //KEYCHAINAPP_SEC_MOD_LINUX_HPP

