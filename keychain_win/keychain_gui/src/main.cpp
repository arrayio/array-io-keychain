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

#include <keychain_lib/secmod_parser_cmd.hpp>

#pragma comment(lib, "advapi32.lib") //TODO: this is bad practice, need to add this depenedencies to CMakeLists.txt
#pragma comment(lib, "Userenv.lib") //TODO: this is bad practice, need to add this depenedencies to CMakeLists.txt
#pragma comment(lib, "Wtsapi32.lib") //TODO: this is bad practice, need to add this depenedencies to CMakeLists.txt

#define DESKTOP_ALL (DESKTOP_READOBJECTS | DESKTOP_CREATEWINDOW | \
DESKTOP_CREATEMENU | DESKTOP_HOOKCONTROL | DESKTOP_JOURNALRECORD | \
DESKTOP_JOURNALPLAYBACK | DESKTOP_ENUMERATE | DESKTOP_WRITEOBJECTS | \
DESKTOP_SWITCHDESKTOP | STANDARD_RIGHTS_REQUIRED)

HDESK hOldDesktop, hNewDesktop;

using namespace keychain_app;
using secmod_commands::secmod_parser_f;

int main(int argc, char *argv[])
{
	//hNewDesktop = OpenDesktopW(_T("secdesktop"), NULL, FALSE, GENERIC_ALL); //GetThreadDesktop(GetCurrentThreadId());
	//hOldDesktop = OpenDesktopW(_T("default"), NULL, FALSE, GENERIC_ALL);
	//SwitchDesktop(hNewDesktop);

	//SetThreadDesktop(hNewDesktop);

	QString srcTrans(argv[1]);

	//QString inputJson("{\"json\":true,\"blockchain\":\"ethereum\",\"data\":{\"nonce\":\"143\",\"gasPrice\":\"5300000000\",\"gas\":\"100000\",\"chainid\":1,\"from\":\"\",\"to\":\"843fcaaeb0cce5ffaf272f5f2ddfff3603f9c2a0\",\"value\":\"173117678552668600\"}}");

	//QString swapInputJson("{\"json\":true,\"blockchain\":\"ethereum\",\"data\":{\"nonce\":\"143\",\"gasPrice\":\"5300000000\",\"gas\":\"100000\",\"chainid\":1,\"from\":\"\",\"to\":\"843fcaaeb0cce5ffaf272f5f2ddfff3603f9c2a0\",\"value\":\"173117678552668600\"},\"swap\":{\"action\":\"createSwap\",\"hash\":\"9dfc35e9b351731c7d8dd1f47351627eeffaaa1b000000000000000000000000\",\"address\":\"000000000000000000000000f59284b3e6631c49283d94fb4b09029b9d3f335f\"}}");

	secmod_parser_f cmd_parse;
	auto cmd_type = cmd_parse(srcTrans.toStdString());

	auto unlock_time = cmd_parse.unlock_time(); //check unlock time. If unlock time > 0 print red lock icon with text warning.
	auto is_json = cmd_parse.is_json();//need to check parse success. If json is false > 0 print red lock icon with text warning.
	

	Transaction trans(srcTrans);
	switch (cmd_type)
	{
	case keychain_app::secmod_commands::blockchain_secmod_te::unknown:
	{
		//auto trx_str = cmd_parse.to_raw_tx();//raw hex transaction
	}
		break;
	
	case keychain_app::secmod_commands::blockchain_secmod_te::parse_error:
	{
		//some error msg into log
	}
		break;
	default:
	{
		//some error msg into log
	}
		break;
	}

	//QJsonDocument document = QJsonDocument::fromJson(/*inputJson*//*swapInputJson*/srcTrans.toUtf8());
	//if (document.isObject()) {
	//	trans.read(document.object());
	//}
	//QString expert("{\"json\":false,\"blockchain\":\"ethereum\",\"data\":\"fd1069fd32ab5a1da25b010080841e000000000000071140420f000000000000000000\"}");
	//trans.setExpertMode(expert);
	

	QApplication a(argc, argv);
	keychain_gui_win w(trans);
	w.show();

	a.exec();

	//WaitForSingleObject((Q_HANDLE)a.thread, INFINITE);
	
	//SwitchDesktop(hOldDesktop);
	return 0;
}
