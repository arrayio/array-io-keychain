#pragma once
#include "NamedPipeServer.h"

#include <keychain_lib/keychain_wrapper.hpp>
#include <keychain_lib/pipeline_parser.hpp>

#include <fcntl.h>
#include <io.h>

#include <iostream>
#include <future>

#include "SecureModuleWrapper.h"

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
	auto res = std::async(std::launch::async, [this](FILE* fd_)->int {
		SecureModuleWrapper secureModuleWrapper;
		keychain_invoke_f f = std::bind(&keychain_wrapper, &secureModuleWrapper, std::placeholders::_1);
		pipeline_parser pipe_line_parser_(std::move(f), fd_);
		int res = pipe_line_parser_.run();
		FlushFileBuffers(hPipe);
		DisconnectNamedPipe(hPipe);
		CloseHandle(hPipe);

		return res;
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
};

void NamedPipeServer::ListenPasswdSecureChannel() {
	
		HANDLE hPipe;
		char buffer[1024];
		DWORD dwRead;


		hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\keychainpass"),
			PIPE_ACCESS_DUPLEX,
			PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,   // FILE_FLAG_FIRST_PIPE_INSTANCE is not needed but forces CreateNamedPipe(..) to fail if the pipe already exists...
			1,
			1024 * 16,
			1024 * 16,
			NMPWAIT_USE_DEFAULT_WAIT,
			NULL);
		while (hPipe != INVALID_HANDLE_VALUE)
		{
			if (ConnectNamedPipe(hPipe, NULL) != FALSE)   // wait for someone to connect to the pipe
			{
				while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
				{
					/* add terminating zero */
					buffer[dwRead] = '\0';

					/* do something with data in buffer */
					printf("%s", buffer);
				}
			}

			DisconnectNamedPipe(hPipe);
		}
};