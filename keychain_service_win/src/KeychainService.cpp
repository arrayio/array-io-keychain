#include "KeychainService.h" 
//#include "ThreadPool.h"
#include <ServiceLogger.h>
#include <iostream>

using namespace  std;

KeychainService::KeychainService(int argc, char **argv)
	: QtService<QCoreApplication>(argc, argv, "Qt Interactive ServiceR")
{
	std::cout << "Service consttructor" << endl;
	//ServiceLogger::getLogger().Log("keychain service conc=strctor");
	setServiceDescription("A Qt service with user interface.");
	setServiceFlags(QtServiceBase::Default);
	_server = new NamedPipeServer();
	//QtServiceBase::instance()->logMessage("service constructor");
	//m_fStopping = false;
}

KeychainService::~KeychainService()
{
	//stop();
}

void KeychainService::start()
{
	std::cout << "Service start" << endl;
//#if defined(Q_OS_WIN)
//	if ((QSysInfo::WindowsVersion & QSysInfo::WV_NT_based) &&
//		(QSysInfo::WindowsVersion >= QSysInfo::WV_VISTA)) {
//		logMessage("Service GUI not allowed on Windows Vista. See the documentation for this example for more information.", QtServiceBase::Error);
//		return;
//	}
//#endif
	
	ServiceLogger::getLogger().Log("Start service for listening");
	//while (!m_fStopping)
	//{
		// Perform main service function here...
	std::bind(&NamedPipeServer::ListenChannel, _server);
	auto server_result = std::async(std::launch::async, &NamedPipeServer::ListenChannel, _server);
	std::cout << "pipeline started";
	ServiceLogger::getLogger().Log("pipeserver has been started");
	//}
	
	//qApp->setQuitOnLastWindowClosed(false);

	/*gui = new QLabel("Service window", 0, Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
	gui->move(QApplication::desktop()->availableGeometry().topLeft());
	gui->show();*/
}

void KeychainService::stop()
{
	//m_fStopping = true;
	//delete gui;
}

void KeychainService::start_server()
{
	_server->ListenChannel();
}


//void KeychainService::processCommand(int code)
//{
	//gui->setText("Command code " + QString::number(code));
	//gui->adjustSize();
	
//}