#pragma once
#include "NamedPipeServer.h"

#include <keychain_lib/keychain_wrapper.hpp>
#include <keychain_lib/pipeline_parser.hpp>

#include <fcntl.h>
#include <io.h>

#include <iostream>
#include <future>

#include "SecureModuleWrapper.h"
#include <ServiceLogger.h>

NamedPipeServer::NamedPipeServer() {
	//_secManage = new SecurityManager();
}

NamedPipeServer::~NamedPipeServer() {

}

using namespace keychain_app;

void NamedPipeServer::ListenChannel(/*LPTSTR channelName*/) {
	
	lpszPipename = (LPTSTR)__TEXT("\\\\.\\pipe\\keychainservice");//channelName;
	ServiceLogger::getLogger().Log("Creating NamedPipe object");
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

	ServiceLogger::getLogger().Log("NamedPipe object is created");
	fConnected = ConnectNamedPipe(hPipe, NULL) ?
		TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
	ServiceLogger::getLogger().Log("ConnectingNamePipe");
	if (!fConnected)
	{
		ServiceLogger::getLogger().Log("Connection error");
		ServiceLogger::getLogger().Log(std::to_string(GetLastError()));
		// The client could not connect, so close the pipe. 
		CloseHandle(hPipe);
		return;
	}			
		
	auto fd = _open_osfhandle(reinterpret_cast<intptr_t>(hPipe), _O_APPEND | _O_RDWR);
		
	ServiceLogger::getLogger().Log("Client connected, creating a processing thread."); //std::cout << "Client connected, creating a processing thread." << std::endl;
	auto res = std::async(std::launch::async, [this](int fd)->int {
		SecureModuleWrapper secureModuleWrapper;
		keychain_invoke_f f = std::bind(&keychain_wrapper, &secureModuleWrapper, std::placeholders::_1);
		pipeline_parser pipe_line_parser_(std::move(f), fd, fd);
		int res = pipe_line_parser_.run();
		FlushFileBuffers(hPipe);
		DisconnectNamedPipe(hPipe);
		CloseHandle(hPipe);
		return res;
	}, fd);
	try
	{
		if (res.get() == -1)
			ServiceLogger::getLogger().Log("Error: cannot read from pipe"); //std::cerr << "Error: cannot read from pipe" << std::endl;
	}
	catch (const std::exception& e)
	{
		ServiceLogger::getLogger().Log("Error: cannot read from pipe");//std::cerr << e.what() << std::endl;
		ServiceLogger::getLogger().Log(e.what());
	}	
};
