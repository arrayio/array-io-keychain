
#include "SecureModuleWrapper.h"
#include "NamedPipeServer.h"
#include <sddl.h>
#include <algorithm>

#pragma comment(lib, "advapi32.lib")

SecurityManager _secman;

SecureModuleWrapper::~SecureModuleWrapper()
{
	//TODO: need implementation
}

keychain_app::byte_seq_t SecureModuleWrapper::get_passwd_trx(const std::string& json_cmd) const
{
	return _startSecureDesktop(json_cmd);
}

keychain_app::byte_seq_t SecureModuleWrapper::get_passwd_unlock(const std::string& keyname, int unlock_time) const
{
	//TODO: need to implement
	//it is experimental future
	//need ot print red lock icon on user dialog window
	return keychain_app::byte_seq_t();
}

keychain_app::byte_seq_t SecureModuleWrapper::get_passwd_on_create() const
{
	return _startSecureDesktop("Please enter password for your new key");
}

void SecureModuleWrapper::print_mnemonic(const string_list& mnemonic) const
{
	//TODO: need implementation
}

keychain_app::byte_seq_t SecureModuleWrapper::_startSecureDesktop(const std::string& str) const
{
	std::vector<char> result_pass;
	result_pass.reserve(512);
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
		throw std::runtime_error("Error: can't receive password: ConvertStringSecurityDescriptorToSecurityDescriptor error");
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
		throw std::runtime_error("Error: can't receive password: INVALID_HANDLE_VALUE");

    const int MAX_WAIT_TIME = 1000;
    /*if (WaitForSingleObject(hPipe, MAX_WAIT_TIME) == WAIT_OBJECT_0)
    {
        FlushFileBuffers(hPipe);
        DisconnectNamedPipe(hPipe);
        CloseHandle(hPipe);
        throw std::runtime_error("Cannot connect to pipe");
    }
    else
    {*/
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
                    result_pass.resize(DataVerify.cbData);
                    std::strncpy(result_pass.data(), (char*)DataVerify.pbData, result_pass.size());
                    //printf("The description of the data was: %S\n", pDescrOut);
                }
                else {
                    DWORD lastError = GetLastError();
                }
            }
            FlushFileBuffers(hPipe);
            DisconnectNamedPipe(hPipe);
            CloseHandle(hPipe);
		};

	//}
	return result_pass;
}