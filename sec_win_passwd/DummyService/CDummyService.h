#pragma once
#include "ServiceBase.h"
class CDummyService :
	public ServiceBase
{
public:

	CDummyService(PWSTR pszServiceName,
		BOOL fCanStop = TRUE,
		BOOL fCanShutdown = TRUE,
		BOOL fCanPauseContinue = FALSE);
	virtual ~CDummyService(void);

protected:

	virtual void OnStart(DWORD dwArgc, PWSTR *pszArgv);
	virtual void OnStop();

	void ServiceWorkerThread(void);

private:

	BOOL m_fStopping;
	HANDLE m_hStoppedEvent;
};

