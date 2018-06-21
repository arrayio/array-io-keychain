#include "SecureModuleWrapper.h"
#include "NamedPipeServer.h"
#include <sddl.h>

#pragma comment(lib, "advapi32.lib")

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
	HANDLE hPipe;
	char buffer[1024];
	DWORD dwRead;
	_secman.CreateSecureDesktop(L"test_transaction_Id");
	SECURITY_ATTRIBUTES  sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = FALSE;
	TCHAR * szSD = TEXT("D:")       // Discretionary ACL
		TEXT("(D;OICI;GA;;;BG)")     // Deny access to 
									 // built-in guests
		TEXT("(D;OICI;GA;;;AN)")     // Deny access to 
									 // anonymous logon
		TEXT("(A;OICI;GRGWGX;;;AU)") // Allow 
									 // read/write/execute 
									 // to authenticated 
									 // users
		TEXT("(A;OICI;GA;;;BA)");    // Allow full control 
									 // to administrators,
	PSECURITY_DESCRIPTOR sd;
	ConvertStringSecurityDescriptorToSecurityDescriptor(
		szSD,
		SDDL_REVISION_1,
		&sd,
		NULL
	);
	sa.lpSecurityDescriptor = &sd;
	hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\keychainpass"),
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,   // FILE_FLAG_FIRST_PIPE_INSTANCE is not needed but forces CreateNamedPipe(..) to fail if the pipe already exists...
		1,
		1024 * 16,
		1024 * 16,
		NMPWAIT_USE_DEFAULT_WAIT,
		&sa);
	while (hPipe != INVALID_HANDLE_VALUE)
	{
		if (ConnectNamedPipe(hPipe, NULL) != FALSE)   // wait for someone to connect to the pipe
		{
			while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
			{
				buffer[dwRead] = '\0';
				LPWSTR pDescrOut = NULL;
				DATA_BLOB DataOut;
				DataOut.cbData = sizeof(buffer) - 1;
				DataOut.pbData = (BYTE*)buffer;
				DATA_BLOB DataVerify;
				if (CryptUnprotectData(
					&DataOut,
					&pDescrOut,
					NULL,                 // Optional entropy
					NULL,                 // Reserved
					NULL,        // Optional PromptStruct
					CRYPTPROTECT_LOCAL_MACHINE,
					&DataVerify))
				{
					//here is decrypted password
					printf("The decrypted data is: %s\n", DataVerify.pbData);
					//printf("The description of the data was: %S\n", pDescrOut);
				}
				else {
					DWORD lastError = GetLastError();
				}
			}
		}
	}
	DisconnectNamedPipe(hPipe);
	return std::wstring(L"blank_password");
}

void SecureModuleWrapper::print_mnemonic(const string_list& mnemonic) const
{
	//TODO: need implementation
}

std::string SecureModuleWrapper::get_uid() const
{
	//TODO: need implementation
	//
	return std::string("user_sid");
}

