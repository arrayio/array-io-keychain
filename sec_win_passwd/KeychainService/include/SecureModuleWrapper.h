#pragma once

#include <keychain_lib/keychain_wrapper.hpp>

class SecureModuleWrapper : public keychain_app::secure_dlg_mod_base
{
public:
	virtual ~SecureModuleWrapper();
	virtual std::wstring get_passwd_trx_raw(const std::string& raw_trx) const override;
	virtual std::wstring get_passwd_trx(const graphene::chain::transaction& trx) const override;
	virtual std::wstring get_passwd(const std::string& str) const override;
	virtual void print_mnemonic(const string_list& mnemonic) const override;
	virtual std::string get_uid() const override;
private:
};
