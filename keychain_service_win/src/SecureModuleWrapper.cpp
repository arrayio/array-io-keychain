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
	_secman.CreateSecureDesktop(str);
	//initializing security attributes
	SECURITY_ATTRIBUTES  sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = FALSE;
	//creating DACL
	if (!ConvertStringSecurityDescriptorToSecurityDescriptor(
		L"D:(D;OICI;GA;;;BG)(D;OICI;GA;;;AN)(A;OICI;GRGWGX;;;AU)(A;OICI;GA;;;BA)",
		SDDL_REVISION_1,
		&(sa.lpSecurityDescriptor),
		NULL
	))
		return std::wstring(L"");
	hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\keychainpass"),
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,   // FILE_FLAG_FIRST_PIPE_INSTANCE is not needed but forces CreateNamedPipe(..) to fail if the pipe already exists...
		1,
		1024 * 16,
		1024 * 16,
		NMPWAIT_USE_DEFAULT_WAIT,
		&sa);
	std::vector<wchar_t> password(256, 0x00);
	if (hPipe == INVALID_HANDLE_VALUE)
		return std::wstring(L"");
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
				std::array<char, 512> _gotpass;
				memset(_gotpass.data(), 0x00, 512);
				assert(DataVerify.cbData <= _gotpass.size() - 1);
				if(DataVerify.cbData <= _gotpass.size() - 1)
					std::strncpy(_gotpass.data(), (char*)DataVerify.pbData, DataVerify.cbData);
				else
					std::strncpy(_gotpass.data(), (char*)DataVerify.pbData, _gotpass.size());
				size_t outSize;
				mbstowcs_s(&outSize, password.data(), password.size(), _gotpass.data(), password.size());
				password.resize(outSize);
				//printf("The description of the data was: %S\n", pDescrOut);
			}
			else {
				DWORD lastError = GetLastError();
			}
		}
		FlushFileBuffers(hPipe);
		DisconnectNamedPipe(hPipe);
		CloseHandle(hPipe);
			
	}
	return std::wstring(password.data(), password.size());
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

