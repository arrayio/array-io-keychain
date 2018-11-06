#ifndef _NAMED_PIPE_SERVER_H
#define _NAMED_PIPE_SERVER_H
#include <windows.h> 
#include <stdio.h> 
#include <tchar.h>
#include <strsafe.h>
#include <thread>

#define BUFSIZE 512

static DWORD WINAPI InstanceThread(LPVOID lpvParam);
static VOID GetAnswerToRequest(LPTSTR pchRequest, LPTSTR pchReply, LPDWORD pchBytes);

class NamedPipeServer {
public: 
	NamedPipeServer();
	~NamedPipeServer();
	void ListenChannel(void);
private:
	BOOL fConnected = FALSE;
	DWORD  dwThreadId = 0;
	HANDLE hThread = NULL;
	HANDLE hPipe = INVALID_HANDLE_VALUE;
	LPTSTR lpszPipename;
};
#endif