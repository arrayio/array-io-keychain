#ifndef DUMMY_SERVICE_H
#define DUMMY_SERVICE_H
#include "ServiceBase.h" 
#include "NamedPipeServer.h" 


class KeychainService : public CServiceBase
{
public:

	KeychainService(PWSTR pszServiceName,
		BOOL fCanStop = TRUE,
		BOOL fCanShutdown = TRUE,
		BOOL fCanPauseContinue = FALSE);
	virtual ~KeychainService(void);

protected:

	virtual void OnStart(DWORD dwArgc, PWSTR *pszArgv);
	virtual void OnStop();

	void ServiceWorkerThread(void);
	void KeychainService::ServiceGetPassThread(void);

private:

	volatile BOOL m_fStopping;
	HANDLE m_hStoppedEvent;
};

#endif