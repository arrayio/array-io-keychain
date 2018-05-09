#pragma once
#ifndef _NAMED_PIPE_SERVER_H
#define _NAMED_PIPE_SERVER_H
#endif
#include <windows.h> 
#include <stdio.h> 
#include <tchar.h>
#include <strsafe.h>
#include "SecurityManager.h"

#define BUFSIZE 512

static DWORD WINAPI InstanceThread(LPVOID lpvParam);
static VOID GetAnswerToRequest(LPTSTR pchRequest, LPTSTR pchReply, LPDWORD pchBytes);

class NamedPipeServer {
public: 
	NamedPipeServer();
	~NamedPipeServer();
	void ListenChannel();
private:
	BOOL fConnected = FALSE;
	DWORD  dwThreadId = 0;
	HANDLE hPipe = INVALID_HANDLE_VALUE;
	HANDLE hThread = NULL;
	LPTSTR lpszPipename;
	SecurityManager _secManage;
};