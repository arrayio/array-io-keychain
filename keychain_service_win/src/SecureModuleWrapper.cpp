#include "SecureModuleWrapper.h"

SecurityManager _secman;

SecureModuleWrapper::~SecureModuleWrapper()
{
	//TODO: need implementation
}

std::wstring SecureModuleWrapper::get_passwd_trx_raw(const std::string& raw_trx) const
{
	//TODO: need implementation
	return std::wstring(L"blank_password");
}

std::wstring SecureModuleWrapper::get_passwd_trx(const graphene::chain::transaction& trx) const
{
	//TODO: need implementation
	return std::wstring(L"blank_password");
}

std::wstring SecureModuleWrapper::get_passwd(const std::string& str) const
{
	//TODO: need implementation

	return std::wstring(L"blank_password");
}

void SecureModuleWrapper::print_mnemonic(const string_list& mnemonic) const
{
	//TODO: need implementation
}

std::string SecureModuleWrapper::get_uid() const
{
	//TODO: need implementation
	_secman.CreateSecureDesktop(L"test_transaction_Id");
	return std::string("user_sid");
}
