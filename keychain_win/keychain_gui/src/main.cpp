#include "keychain_gui_win.h"
#include <QtWidgets/QApplication>
#include <windows.h>
#include <UserEnv.h>
#include <Tchar.h>
#include <WtsApi32.h>
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
	SwitchDesktop(hNewDesktop);

	SetThreadDesktop(hNewDesktop);
	QApplication a(argc, argv);
	keychain_gui_win w;
	w.show();

	a.exec();

	//WaitForSingleObject((Q_HANDLE)a.thread, INFINITE);
	
	SwitchDesktop(hOldDesktop);
	return 0;
}
