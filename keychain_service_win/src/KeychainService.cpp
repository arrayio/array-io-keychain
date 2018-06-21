#include "KeychainService.h" 
#include "ThreadPool.h"
#include <ServiceLogger.h>


KeychainService::KeychainService(PWSTR pszServiceName,
	BOOL fCanStop,
	BOOL fCanShutdown,
	BOOL fCanPauseContinue)
	: CServiceBase(pszServiceName, fCanStop, fCanShutdown, fCanPauseContinue)
{
	m_fStopping = FALSE;

	m_hStoppedEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (m_hStoppedEvent == NULL)
	{
		throw GetLastError();
	}
}


KeychainService::~KeychainService(void)
{
	if (m_hStoppedEvent)
	{
		CloseHandle(m_hStoppedEvent);
		m_hStoppedEvent = NULL;
	}
}

void KeychainService::OnStart(DWORD dwArgc, LPWSTR *lpszArgv)
{
	// Log a service start message to the Application log. 
	WriteEventLogEntry((PWSTR)"KeychainService in OnStart",
		EVENTLOG_INFORMATION_TYPE);
	// Queue the main service function for execution in a worker thread. 
	CThreadPool::QueueUserWorkItem(&KeychainService::ServiceWorkerThread, this);
}

void KeychainService::ServiceWorkerThread(void)
{
	// Periodically check if the service is stopping. 
	NamedPipeServer _server;
	ServiceLogger::getLogger().Log("Start service for listening");
	while (!m_fStopping)
	{
		// Perform main service function here...
		_server.ListenChannel();
	}
	ServiceLogger::getLogger().Log("Stop service for listening");
	// Signal the stopped event. 
	SetEvent(m_hStoppedEvent);
	_server.~NamedPipeServer();
}

void KeychainService::OnStop()
{
	WriteEventLogEntry((PWSTR)"KeychainService in OnStop",
		EVENTLOG_INFORMATION_TYPE);
	m_fStopping = TRUE;
	
}
