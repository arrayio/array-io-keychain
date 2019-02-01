
#include "SecureModuleWrapper.h"
#include "NamedPipeServer.h"
#include <keychain_lib/secmod_parser_cmd.hpp>
#include <sddl.h>
#include <algorithm>

#pragma comment(lib, "advapi32.lib")

namespace sm_cmd = keychain_app::secmod_commands;

SecurityManager _secman;

SecureModuleWrapper::~SecureModuleWrapper()
{
	//TODO: need implementation
}

std::string SecureModuleWrapper::exec_cmd(const std::string& json_cmd) const
{
  sm_cmd::secmod_parser_f parser;
  auto etype = parser(json_cmd);
  int unlock_time = 0;
  switch (etype)
  {
    case sm_cmd::events_te::sign_hex:
    {
      auto cmd = parser.params<sm_cmd::events_te::sign_hex>();
      unlock_time = cmd.unlock_time;
    }
    break;
    case sm_cmd::events_te::unlock:
    {
      auto cmd = parser.params<sm_cmd::events_te::unlock>();
      unlock_time = cmd.unlock_time;
    }
    break;
  }

	keychain_app::byte_seq_t result_pass;
	result_pass.reserve(512);
	HANDLE hPipe;
	char buffer[1024];
	DWORD dwRead;
#if 0
	HANDLE transactionPipe;

	auto& log = logger_singleton::instance();
	BOOST_LOG_SEV(log.lg, info) << "Send to pipe:"+ json_cmd;
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
		FC_LIGHT_THROW_EXCEPTION(fc_light::password_input_exception,
			"Can't receive password: ConvertStringSecurityDescriptorToSecurityDescriptor error");

	transactionPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\transpipe"),
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT, 
		1,
		9000 * 16,
		9000 * 16,
		NMPWAIT_USE_DEFAULT_WAIT,
		&sa);

	DWORD writtenSize;
	DWORD lastErrror;
	DisconnectNamedPipe(transactionPipe);
	_secman.CreateSecureDesktop(unlock_time);
	while (transactionPipe != INVALID_HANDLE_VALUE) {
		DWORD dwWritten;
		if (ConnectNamedPipe(transactionPipe, NULL) != FALSE)   // wait for someone to connect to the pipe
		{
			std::string trans = json_cmd;
			BOOST_LOG_SEV(log.lg, info) << "Send to pipe: (hardcode)" + trans;
			trans.push_back('\0');
			WriteFile(transactionPipe,
				trans.c_str(),
				trans.length(),   // = length of string + terminating '\0' !!!
				&dwWritten,
				NULL);
			BOOST_LOG_SEV(log.lg, info) << "Error code: " << GetLastError();
			CloseHandle(transactionPipe);
			DisconnectNamedPipe(transactionPipe);
			break;
		}
		//lastErrror = GetLastError();
		//throw std::runtime_error("Error: can't write pipe transaction");
	}

	
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
		FC_LIGHT_THROW_EXCEPTION(fc_light::password_input_exception,"Can't receive password: INVALID_HANDLE_VALUE");

  constexpr int MAX_WAIT_TIME = 1000;
   
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
              //printf("The decrypted data is: %s\n", DataVerify.pbData);
              result_pass.resize(DataVerify.cbData);
              std::strncpy(result_pass.data(), (char*)DataVerify.pbData, result_pass.size());
		std::string resPass(result_pass.data());
		if (resPass.find("cancel_pass_enterance") != -1) {
			result_pass.resize(0);
		}
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
#endif
  std::string result;
  result_pass = { 'b', 'l', 'a', 'n', 'k' };
  sm_cmd::secmod_resonse_common response;
  if (result_pass.empty())
  {
    response.etype = sm_cmd::response_te::null;
    result = fc_light::json::to_pretty_string(response);
  }    
  else
  {
    response.etype = sm_cmd::response_te::password;
    response.params = result_pass;
    result = fc_light::json::to_pretty_string(response);
  }    
	return result;
}