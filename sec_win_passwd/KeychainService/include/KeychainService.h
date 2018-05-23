#ifndef DUMMY_SERVICE_H
#define DUMMY_SERVICE_H
#include "ServiceBase.h" 


class DummyService : public CDummyBase
{
public:

	DummyService(PWSTR pszServiceName,
		BOOL fCanStop = TRUE,
		BOOL fCanShutdown = TRUE,
		BOOL fCanPauseContinue = FALSE);
	virtual ~DummyService(void);

protected:

	virtual void OnStart(DWORD dwArgc, PWSTR *pszArgv);
	virtual void OnStop();

	void ServiceWorkerThread(void);

private:

	BOOL m_fStopping;
	HANDLE m_hStoppedEvent;
};

#endif