#include "keymanager_dialog.hpp"
#include <QtWidgets/QApplication>
#include <QJsonDocument>
//#include "Transaction.h"
#include <QString>
#include <cwchar>
#include <keychain_lib/keychain_logger.hpp>
#include <keychain_lib/version_info.hpp>
#include <keychain_lib/secmod_parser_cmd.hpp>



using namespace std;
using namespace keychain_app;
using secmod_commands::secmod_parser_f;

int main(int argc, char *argv[])
{
	//auto log = logger_singleton::instance("keychain_gui");
	//BOOST_LOG_SEV(log.lg, info) << "KeyChain key manager app" << keychain_app::version_info::version();
//#ifdef FROMPROCCESS
//	DWORD dwWritten;
//	char buffer[9000];
//	DWORD dwRead = 0;
//	HANDLE transPipe = CreateFile(TEXT("\\\\.\\pipe\\transpipe"),
//		GENERIC_READ | GENERIC_WRITE,
//		0,
//		NULL,
//		OPEN_EXISTING,
//		0,
//		NULL);
//	BOOST_LOG_SEV(log.lg, info) << GetLastError();
//	if (transPipe != INVALID_HANDLE_VALUE)
//	{
//		ReadFile(transPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL);
//		CloseHandle(transPipe);
//	}
//	hOldDesktop = OpenDesktop(_T("default"), NULL, FALSE, GENERIC_ALL);
//	hNewDesktop = OpenDesktopW(_T("secdesktop"), NULL, FALSE, GENERIC_ALL); //GetThreadDesktop(GetCurrentThreadId());
//	SwitchDesktop(hNewDesktop);
//	if (hNewDesktop == NULL) {
//		hNewDesktop = CreateDesktop(L"secdesktop", NULL, NULL, 0, DESKTOP_ALL, NULL);
//		BOOST_LOG_SEV(log.lg, info) << "SecurityDescktop" << GetLastError();
//	}
//	if (hOldDesktop == NULL)
//		BOOST_LOG_SEV(log.lg, info) <<"olddescktop" << GetLastError();
//	SwitchDesktop(hNewDesktop);
//	SetThreadDesktop(hNewDesktop);
//#endif
//	int endIndex = -1;
//	QString srcTrans;
//#ifdef FROMPROCCESS
//	for (int i = 0; i < dwRead; i++) {
//		srcTrans.push_back(buffer[i]);
//	}
//#endif
	

	QApplication a(argc, argv);
	keymanager_dialog manager;
	manager.show();

	a.exec();

//#ifdef FROMPROCCESS
//	SwitchDesktop(hOldDesktop);
//#endif
	return 0;
}
