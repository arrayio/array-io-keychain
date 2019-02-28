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
#include <keychain_lib/keychain_logger.hpp>
#include <keychain_lib/version_info.hpp>
#include <keychain_lib/secmod_parser_cmd.hpp>

#include "EventsHandlerSingleton.h"

#pragma comment(lib, "advapi32.lib") //TODO: this is bad practice, need to add this depenedencies to CMakeLists.txt
#pragma comment(lib, "Userenv.lib") //TODO: this is bad practice, need to add this depenedencies to CMakeLists.txt
#pragma comment(lib, "Wtsapi32.lib") //TODO: this is bad practice, need to add this depenedencies to CMakeLists.txt

#define DESKTOP_ALL (DESKTOP_READOBJECTS | DESKTOP_CREATEWINDOW | \
DESKTOP_CREATEMENU | DESKTOP_HOOKCONTROL | DESKTOP_JOURNALRECORD | \
DESKTOP_JOURNALPLAYBACK | DESKTOP_ENUMERATE | DESKTOP_WRITEOBJECTS | \
DESKTOP_SWITCHDESKTOP | STANDARD_RIGHTS_REQUIRED)
#define FROMPROCCESS
HDESK hOldDesktop, hNewDesktop;

using namespace std;
using namespace keychain_app;
using secmod_commands::secmod_parser_f;

int main(int argc, char *argv[])
{
	auto& log = logger_singleton::instance("keychain_gui");
	BOOST_LOG_SEV(log.lg, info) << "KeyChain gui application started, version = " << keychain_app::version_info::version();
#ifdef FROMPROCCESS
	DWORD dwWritten;
	char buffer[9000];
  memset(buffer, 0x00, 9000);
	DWORD dwRead = 0;
	HANDLE transPipe = CreateFile(TEXT("\\\\.\\pipe\\transpipe"),
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	BOOST_LOG_SEV(log.lg, info) << GetLastError();
	if (transPipe != INVALID_HANDLE_VALUE)
	{
		ReadFile(transPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL);
		CloseHandle(transPipe);
	}
	hOldDesktop = OpenDesktop(_T("default"), NULL, FALSE, GENERIC_ALL);
	hNewDesktop = OpenDesktopW(_T("secdesktop"), NULL, FALSE, GENERIC_ALL); //GetThreadDesktop(GetCurrentThreadId());
	SwitchDesktop(hNewDesktop);
	if (hNewDesktop == NULL) {
		hNewDesktop = CreateDesktop(L"secdesktop", NULL, NULL, 0, DESKTOP_ALL, NULL);
		BOOST_LOG_SEV(log.lg, info) << "SecurityDescktop" << GetLastError();
	}
	if (hOldDesktop == NULL)
		BOOST_LOG_SEV(log.lg, info) <<"olddescktop" << GetLastError();
	SwitchDesktop(hNewDesktop);
	SetThreadDesktop(hNewDesktop);
#endif
	BOOST_LOG_SEV(log.lg, info) << "Got from pipe: " << buffer;

//  std::string input = "{\"etype\":\"sign_trx\",\"params\":{\"keyname\":\"test1\",\"is_parsed\":false,\"blockchain\":\"unknown\",\"unlock_time\":45,\"trx_view\":\"871689d060721b5cec5a010080841e00000000000011130065cd1d0000000000000000\"}}";

  QApplication a(argc, argv);
  parseSecmodEvents(std::string(buffer));
	
	a.exec();

#ifdef FROMPROCCESS
	SwitchDesktop(hOldDesktop);
#endif
	return 0;
}
