#pragma once
#include "NamedPipeServer.h"
#include "SecurityManager.h"

#include <keychain_lib/keychain_wrapper.hpp>
#include <keychain_lib/secure_module_singletone.hpp>
#include <keychain_lib/pipeline_parser.hpp>

#include <fcntl.h>
#include <io.h>

#include <iostream>
#include <future>

class sec_mod_dummy : public keychain_app::secure_dlg_mod_base
{
public:
	sec_mod_dummy();
	virtual ~sec_mod_dummy();
	virtual std::string get_passwd_trx_raw(const std::string& raw_trx) const override;
	virtual std::string get_passwd_trx(const graphene::chain::transaction& trx) const override;
	virtual std::string get_passwd(const std::string& str) const override;
	virtual void print_mnemonic(const string_list& mnemonic) const override;
	virtual std::string get_uid() const override;

};

sec_mod_dummy::sec_mod_dummy()
{}

sec_mod_dummy::~sec_mod_dummy()
{}

std::string sec_mod_dummy::get_passwd_trx(const graphene::chain::transaction& trx) const
{
	return std::string("blank_password");
}

std::string sec_mod_dummy::get_uid() const
{
	return std::string("uid");
}

void sec_mod_dummy::print_mnemonic(const string_list& mnemonic) const
{

}

std::string sec_mod_dummy::get_passwd_trx_raw(const std::string& raw_trx) const
{
	return std::string("blank_password");
}

std::string sec_mod_dummy::get_passwd(const std::string& str) const
{
	return std::string("blank_password");
}

NamedPipeServer::NamedPipeServer() {
	//_secManage = new SecurityManager();
}

NamedPipeServer::~NamedPipeServer() {

}

using namespace keychain_app;

void NamedPipeServer::ListenChannel(/*LPTSTR channelName*/) {
	
	lpszPipename = (LPTSTR)__TEXT("\\\\.\\pipe\\keychainservice");//channelName;
	
	hPipe = CreateNamedPipe(
		lpszPipename,             // pipe name 
		PIPE_ACCESS_DUPLEX,       // read/write access 
		PIPE_TYPE_MESSAGE |       // message type pipe 
		PIPE_READMODE_MESSAGE |   // message-read mode 
		PIPE_WAIT,                // blocking mode 
		PIPE_UNLIMITED_INSTANCES, // max. instances  
		BUFSIZE,                  // output buffer size 
		BUFSIZE,                  // input buffer size 
		0,                        // client time-out 
		NULL);                    // default security attribute 


	// Wait for the client to connect; if it succeeds, 
	// the function returns a nonzero value. If the function
	// returns zero, GetLastError returns ERROR_PIPE_CONNECTED. 

	fConnected = ConnectNamedPipe(hPipe, NULL) ?
		TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
	if (!fConnected)
	{
		// The client could not connect, so close the pipe. 
		CloseHandle(hPipe);
		return;
	}			
		
	auto fd_c = _open_osfhandle(reinterpret_cast<intptr_t>(hPipe), _O_APPEND | _O_RDWR);
	FILE* fd = _fdopen(fd_c, "a+");
		
	std::cout << "Client connected, creating a processing thread." << std::endl;
	auto res = std::async(std::launch::async, [](FILE* fd_)->int {
		const secure_dlg_mod_base* sec_mod = secure_module<sec_mod_dummy>::instance();
		keychain_invoke_f f = std::bind(&keychain_wrapper, sec_mod, std::placeholders::_1);
		pipeline_parser pipe_line_parser_(std::move(f), fd_);
		return pipe_line_parser_.run();
	}, fd);
	try
	{
		if (res.get() == -1)
			std::cerr << "Error: cannot read from pipe" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	DisconnectNamedPipe(hPipe);
	CloseHandle(hPipe);
}

/*
static DWORD WINAPI InstanceThread(LPVOID lpvParam) 
{
	HANDLE hHeap = GetProcessHeap();
	TCHAR* pchRequest = (TCHAR*)HeapAlloc(hHeap, 0, BUFSIZE * sizeof(TCHAR));
	TCHAR* pchReply = (TCHAR*)HeapAlloc(hHeap, 0, BUFSIZE * sizeof(TCHAR));

	DWORD cbBytesRead = 20, cbReplyBytes = 0, cbWritten = 0;
	BOOL fSuccess = FALSE;
	HANDLE hPipe = NULL;

	// Do some extra error checking since the app will keep running even if this
	// thread fails.

	if (lpvParam == NULL)
	{
		printf("\nERROR - Pipe Server Failure:\n");
		printf("   InstanceThread got an unexpected NULL value in lpvParam.\n");
		printf("   InstanceThread exitting.\n");
		if (pchReply != NULL) HeapFree(hHeap, 0, pchReply);
		if (pchRequest != NULL) HeapFree(hHeap, 0, pchRequest);
		return (DWORD)-1;
	}

	if (pchRequest == NULL)
	{
		printf("\nERROR - Pipe Server Failure:\n");
		printf("   InstanceThread got an unexpected NULL heap allocation.\n");
		printf("   InstanceThread exitting.\n");
		if (pchReply != NULL) HeapFree(hHeap, 0, pchReply);
		return (DWORD)-1;
	}

	if (pchReply == NULL)
	{
		printf("\nERROR - Pipe Server Failure:\n");
		printf("   InstanceThread got an unexpected NULL heap allocation.\n");
		printf("   InstanceThread exitting.\n");
		if (pchRequest != NULL) HeapFree(hHeap, 0, pchRequest);
		return (DWORD)-1;
	}

	// Print verbose messages. In production code, this should be for debugging only.
	printf("InstanceThread created, receiving and processing messages.\n");

	// The thread's parameter is a handle to a pipe object instance. 

	hPipe = (HANDLE)lpvParam;
		SecurityManager _secManager;
		_secManager.CreateSecureDesktop();

	// Loop until done reading
	while (1)
	{
		// Read client requests from the pipe. This simplistic code only allows messages
		// up to BUFSIZE characters in length.
		fSuccess = ReadFile(
			hPipe,        // handle to pipe 
			pchRequest,    // buffer to receive data 
			BUFSIZE * sizeof(TCHAR), // size of buffer 
			&cbBytesRead, // number of bytes read 
			NULL);        // not overlapped I/O 

		if (!fSuccess || cbBytesRead == 0)
		{
			if (GetLastError() == ERROR_BROKEN_PIPE)
			{
				_tprintf(TEXT("InstanceThread: client disconnected.\n"), GetLastError());
			}
			else
			{
				_tprintf(TEXT("InstanceThread ReadFile failed, GLE=%d.\n"), GetLastError());
			}
			break;
		}

		// Process the incoming message.
		GetAnswerToRequest(pchRequest, pchReply, &cbReplyBytes);

		// Write the reply to the pipe. 
		fSuccess = WriteFile(
			hPipe,        // handle to pipe 
			pchReply,     // buffer to write from 
			cbReplyBytes, // number of bytes to write 
			&cbWritten,   // number of bytes written 
			NULL);        // not overlapped I/O 

		if (!fSuccess || cbReplyBytes != cbWritten)
		{
			_tprintf(TEXT("InstanceThread WriteFile failed, GLE=%d.\n"), GetLastError());
			break;
		}
	}

	// Flush the pipe to allow the client to read the pipe's contents 
	// before disconnecting. Then disconnect the pipe, and close the 
	// handle to this pipe instance. 

	FlushFileBuffers(hPipe);
	DisconnectNamedPipe(hPipe);
	CloseHandle(hPipe);

	HeapFree(hHeap, 0, pchRequest);
	HeapFree(hHeap, 0, pchReply);

	printf("InstanceThread exitting.\n");
	return 1;
}


static VOID GetAnswerToRequest(LPTSTR pchRequest, LPTSTR pchReply, LPDWORD pchBytes)
{
	_tprintf(TEXT("Client Request String:\"%s\"\n"), pchRequest);

	// Check the outgoing message to make sure it's not too long for the buffer.
	if (FAILED(StringCchCopy(pchReply, BUFSIZE, TEXT("default answer from server"))))
	{
		*pchBytes = 0;
		pchReply[0] = 0;
		printf("StringCchCopy failed, no outgoing message.\n");
		return;
	}
	*pchBytes = (lstrlen(pchReply) + 1) * sizeof(TCHAR);
}
*/