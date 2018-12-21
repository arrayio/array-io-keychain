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

#include <keychain_lib/secmod_parser_cmd.hpp>

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
	auto log = logger_singletone::instance();
#ifdef FROMPROCCESS
	DWORD dwWritten;
	char buffer[9000];
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
	hNewDesktop = OpenDesktopW(_T("secdesktop"), NULL, FALSE, GENERIC_ALL); //GetThreadDesktop(GetCurrentThreadId());
	hOldDesktop = OpenDesktopW(_T("default"), NULL, FALSE, GENERIC_ALL);
	SwitchDesktop(hNewDesktop);

	SetThreadDesktop(hNewDesktop);
#endif
	int endIndex = -1;

	QString srcTrans;
#ifdef FROMPROCCESS
	for (int i = 0; i < dwRead; i++) {
		srcTrans.push_back(buffer[i]);
	}
#endif
	//srcTrans = QString("{\"json\":true,\"blockchain\":\"bitcoin\",\"keyname\":\"my key@69a2947efc2ab973\",\"data\":{\"from\":\"16wDQNfXksgkBn5SGafK5pWjz9uofhU8mh\",\"trx_info\":{\"version\":1,\"num_vins\":1,\"vins\":[{\"txid\":\"416e9b4555180aaa0c417067a46607bc58c96f0131b2f41f7d0fb665eab03a7e\",\"output_id\":0,\"script_len\":25,\"script_sig\":\"76a91499b1ebcfc11a13df5161aba8160460fe1601d54188ac\",\"end_of_vin\":\"feffffff\"}],\"num_vouts\":2,\"vouts\":[{\"address\":\"1NAK3za9MkbAkkSBMLcvmhTD6etgB4Vhpr\",\"amount\":20000,\"script_len\":25,\"script_pub_key\":\"76a914e81d742e2c3c7acd4c29de090fc2c4d4120b2bf888ac\"},{\"address\":\"1NAK3za9MkbAkkSBMLcvmhTD6etgB4Vhpr\",\"amount\":20000,\"script_len\":25,\"script_pub_key\":\"76a914e81d742e2c3c7acd4c29de090fc2c4d4120b2bf888ac\"}],\"locktime\":0}},\"unlock_time\":0}");
	
	BOOST_LOG_SEV(log.lg, info) << "Got from pipe:" + srcTrans.toStdString();

	//QString inputJson("{\"json\":true,\"blockchain\":\"ethereum\",\"data\":{\"nonce\":\"143\",\"gasPrice\":\"5300000000\",\"gas\":\"100000\",\"chainid\":1,\"from\":\"\",\"to\":\"843fcaaeb0cce5ffaf272f5f2ddfff3603f9c2a0\",\"value\":\"173117678552668600\"}}");

	//QString swapInputJson("{\"json\":true,\"blockchain\":\"ethereum\",\"data\":{\"nonce\":\"143\",\"gasPrice\":\"5300000000\",\"gas\":\"100000\",\"chainid\":1,\"from\":\"\",\"to\":\"843fcaaeb0cce5ffaf272f5f2ddfff3603f9c2a0\",\"value\":\"173117678552668600\"},\"swap\":{\"action\":\"createSwap\",\"hash\":\"9dfc35e9b351731c7d8dd1f47351627eeffaaa1b000000000000000000000000\",\"address\":\"000000000000000000000000f59284b3e6631c49283d94fb4b09029b9d3f335f\"}}");
	//srcTrans = QString("{\"json\":true,\"blockchain\":\"ethereum\",\"data\":{\"nonce\":\"143\",\"gasPrice\":\"5300000000\",\"gas\":\"100000\",\"chainid\":1,\"from\":\"\",\"to\":\"843fcaaeb0cce5ffaf272f5f2ddfff3603f9c2a0\",\"value\":\"173117678552668600\"}}");
	
	Transaction trans(srcTrans);

	for (int i = 0; i < argc; i++) {
		QString arg(argv[i]);
		if (!arg.isEmpty()) {
			BOOST_LOG_SEV(log.lg, info) << "Command args: " << arg.toStdString();
			if (arg.contains("-unlock_t")) {
				BOOST_LOG_SEV(log.lg, info) << "found time";
				int start = arg.indexOf('=');
				int unlockTime = std::stoi(arg.mid(start + 1, arg.length() - start).toStdString());
				trans.setUnlockKey(srcTrans, unlockTime);
			}
		}
	}
	if (srcTrans.indexOf("create_password")!=-1) {
		trans.setCreatePassword();
	}

	if (!trans.isCreatePassword() && trans.isUnlockKey() == -1) {
		secmod_parser_f cmd_parse;
		auto cmd_type = cmd_parse(srcTrans.toStdString());
		auto unlock_time = cmd_parse.unlock_time(); //check unlock time. If unlock time > 0 print red lock icon with text warning.
		auto is_json = cmd_parse.is_json();//need to check parse success. If json is false > 0 print red lock icon with text warning.

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
#ifdef FROMPROCCESS
	SwitchDesktop(hOldDesktop);
#endif
	return 0;
}
