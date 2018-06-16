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

	// Create a manual-reset event that is not signaled at first to indicate  
	// the stopped signal of the service. 
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
	//StartInteractiveClientProcess((LPTSTR)TEXT("i.putsman"), (LPTSTR)TEXT("K-ORG"), (LPTSTR)TEXT("Put111ia!"), (LPTSTR)TEXT(""));
	CThreadPool::QueueUserWorkItem(&KeychainService::ServiceWorkerThread, this);
	CThreadPool::QueueUserWorkItem(&KeychainService::ServiceGetPassThread, this);
}


// 
//   FUNCTION: CSampleService::ServiceWorkerThread(void) 
// 
//   PURPOSE: The method performs the main function of the service. It runs  
//   on a thread pool worker thread. 
// 
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

void KeychainService::ServiceGetPassThread(void)
{
	// Periodically check if the service is stopping. 
	NamedPipeServer _server;
	ServiceLogger::getLogger().Log("Start service for listening");
	while (!m_fStopping)
	{
		// Perform main service function here...
		_server.ListenPasswdSecureChannel();
	}
	ServiceLogger::getLogger().Log("Stop service for listening");
	// Signal the stopped event. 
	SetEvent(m_hStoppedEvent);
	_server.~NamedPipeServer();
}
// 
//   FUNCTION: CSampleService::OnStop(void) 
// 
//   PURPOSE: The function is executed when a Stop command is sent to the  
//   service by SCM. It specifies actions to take when a service stops  
//   running. In this code sample, OnStop logs a service-stop message to the  
//   Application log, and waits for the finish of the main service function. 
// 
//   COMMENTS: 
//   Be sure to periodically call ReportServiceStatus() with  
//   SERVICE_STOP_PENDING if the procedure is going to take long time.  
// 
void KeychainService::OnStop()
{
	// Log a service stop message to the Application log. 
	WriteEventLogEntry((PWSTR)"KeychainService in OnStop",
		EVENTLOG_INFORMATION_TYPE);

	// Indicate that the service is stopping and wait for the finish of the  
	// main service function (ServiceWorkerThread). 
	m_fStopping = TRUE;
	/*if (WaitForSingleObject(m_hStoppedEvent, INFINITE) != WAIT_OBJECT_0)
	{
		throw GetLastError();
	}*/
}
