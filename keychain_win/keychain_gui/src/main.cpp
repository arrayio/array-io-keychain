#include "keychain_gui_win.h"
#include <QtWidgets/QApplication>
#include <windows.h>
#include <UserEnv.h>
#include <Tchar.h>
#include <WtsApi32.h>
#include <QJsonDocument>
#include "Transaction.h"
#include <QString>
#include <cwchar>
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "Userenv.lib")
#pragma comment(lib, "Wtsapi32.lib")

#define DESKTOP_ALL (DESKTOP_READOBJECTS | DESKTOP_CREATEWINDOW | \
DESKTOP_CREATEMENU | DESKTOP_HOOKCONTROL | DESKTOP_JOURNALRECORD | \
DESKTOP_JOURNALPLAYBACK | DESKTOP_ENUMERATE | DESKTOP_WRITEOBJECTS | \
DESKTOP_SWITCHDESKTOP | STANDARD_RIGHTS_REQUIRED)

HDESK hOldDesktop, hNewDesktop;

int main(int argc, char *argv[])
{
	hNewDesktop = OpenDesktopW(_T("secdesktop"), NULL, FALSE, GENERIC_ALL); //GetThreadDesktop(GetCurrentThreadId());
	hOldDesktop = OpenDesktopW(_T("default"), NULL, FALSE, GENERIC_ALL);
	//SwitchDesktop(hNewDesktop);

	//SetThreadDesktop(hNewDesktop);

	QString inputJson("{\"json\":true,\"blockchain\":\"ethereum\",\"data\":{\"nonce\":\"143\",\"gasPrice\":\"5300000000\",\"gas\":\"100000\",\"chainid\":1,\"from\":\"\",\"to\":\"843fcaaeb0cce5ffaf272f5f2ddfff3603f9c2a0\",\"value\":\"173117678552668600\"}}");


	Transaction trans;
	QJsonDocument document = QJsonDocument::fromJson(inputJson.toUtf8());
	if (document.isObject()) {
		trans.read(document.object());
	}
	QString expert("{\"json\":false,\"blockchain\":\"ethereum\",\"data\":\"fd1069fd32ab5a1da25b010080841e000000000000071140420f000000000000000000\"}");
	trans.setExpertMode(expert);


	QApplication a(argc, argv);
	keychain_gui_win w;
	w.SetTransaction(trans);
	w.show();

	a.exec();

	//WaitForSingleObject((Q_HANDLE)a.thread, INFINITE);
	
	//SwitchDesktop(hOldDesktop);
	return 0;
}
