//
//  sec_mod_mac.hpp
//  keychain
//
//  Created by Mikhail Lutskiy on 29/10/2018.
//
//#ifdef __OBJC__
//#ifndef sec_mod_mac_hpp
//#define sec_mod_mac_hpp

#include <keychain_lib/keychain.hpp>

namespace keychain_app
{
    
    class sec_mod_mac: public secure_dlg_mod_base
    {
    public:
        sec_mod_mac();
        virtual ~sec_mod_mac();
        virtual std::string exec_cmd(const std::string& json_cmd) const override;
    private:
        static constexpr const char* pass_str = "blank_password";
    };
    
}

//#include <keychain_lib/keychain.hpp>
//
//namespace keychain_app
//{
//    
//    class sec_mod_mac: public secure_dlg_mod_base
//    {
//    public:
//        sec_mod_mac();
//        virtual ~sec_mod_mac();
//        
//        virtual keychain_app::byte_seq_t get_passwd_trx(const std::string& raw_trx) const override;
//        virtual keychain_app::byte_seq_t get_passwd_unlock(const std::string& keyname, int unlock_time) const override;
//        virtual keychain_app::byte_seq_t get_passwd_on_create(const std::string& keyname = std::string("") ) const override;
//        virtual void print_mnemonic(const string_list& mnemonic) const override;
//    private:
//        static constexpr const char* pass_str = "blank_password";
//    };
//    
//}

//#endif //KEYCHAINAPP_SEC_MOD_LINUX_HPP
//#endif
