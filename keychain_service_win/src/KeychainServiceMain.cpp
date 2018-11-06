#include <wchar.h>
#include <thread>
#include <conio.h>
//#include "ServiceInstaller.h"
#include "KeychainService.h"
//#include "ServiceBase.h"
#include <iostream>
//#include <ServiceLogger.h>

using namespace std;

int main(int argc, char **argv)
{
#if !defined(Q_OS_WIN)
	// QtService stores service settings in SystemScope, which normally require root privileges.
	// To allow testing this example as non-root, we change the directory of the SystemScope settings file.
	QSettings::setPath(QSettings::NativeFormat, QSettings::SystemScope, QDir::tempPath());
	qWarning("(Example uses dummy settings file: %s/QtSoftware.conf)", QDir::tempPath().toLatin1().constData());
#endif
	//ServiceLogger::getLogger().Log("main soft thread");
	std::cout << "Service start" << endl;
	KeychainService service(argc, argv);
	service.exec();
}