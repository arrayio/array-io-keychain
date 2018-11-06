#ifndef KEYCHAIN_SERVICE_H
#define KEYCHAIN_SERVICE_H
#include "ServiceBase.h" 
#include "NamedPipeServer.h" 
#include <QApplication>
#include <QDesktopWidget>
#include <QLabel>
#include <QObject>
#include <QDir>
#include <QSettings>
#include <QtService>
#include <future>

using namespace std;

class KeychainService : public QtService<QCoreApplication>
{
	
	public:
		KeychainService(int argc, char **argv);
		~KeychainService();

	protected:

		void start();
		void stop();
		//void pause();
		//void resume();
		//void processCommand(int code);

	private:
		//Q_OBJECT
		void start_server();
		NamedPipeServer *_server;
		//QLabel * gui;
		//bool m_fStopping;
};

//class KeychainService : public CServiceBase
//{
//public:
//
//	KeychainService(PWSTR pszServiceName,
//		BOOL fCanStop = TRUE,
//		BOOL fCanShutdown = TRUE,
//		BOOL fCanPauseContinue = FALSE);
//	virtual ~KeychainService(void);
//
//protected:
//
//	virtual void OnStart(DWORD dwArgc, PWSTR *pszArgv);
//	virtual void OnStop();
//
//	void ServiceWorkerThread(void);
//	void KeychainService::ServiceGetPassThread(void);
//
//private:
//
//	volatile BOOL m_fStopping;
//	HANDLE m_hStoppedEvent;
//};

#endif