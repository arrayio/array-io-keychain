#include "keymanager_dialog.hpp"
#include <QtWidgets/QApplication>
#include <QJsonDocument>
//#include "Transaction.h"
#include <QString>
#include <cwchar>
#include <keychain_lib/keychain_logger.hpp>
#include <keychain_lib/secmod_parser_cmd.hpp>

using namespace std;
using namespace keychain_app;

using secmod_commands::secmod_parser_f;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	keymanager_dialog manager;
	manager.init();
	manager.show();

	a.exec();
	return 0;
}
