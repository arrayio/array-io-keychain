#include "keymanager_dialog.hpp"
#include <QtWidgets/QApplication>
#include <QJsonDocument>
//#include "Transaction.h"
#include <QString>
#include <QtGlobal>
#include <cwchar>
#include <keychain_lib/keychain_logger.hpp>
#include <keychain_lib/secmod_parser_cmd.hpp>

using namespace std;
using namespace keychain_app;

namespace attrs = boost::log::attributes;

using secmod_commands::secmod_parser_f;

static const QtMessageHandler QT_DEFAULT_MESSAGE_HANDLER = qInstallMessageHandler(0);

void HandleLoggingOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    auto log = logger_singleton::instance();
    switch (type) {
    case QtDebugMsg:
        BOOST_LOG_SEV(log.lg, debug) << "Debug: %s (%s:%u, %s)\n" << localMsg.constData() << context.file << context.line << context.function;
        break;
    case QtInfoMsg:
        BOOST_LOG_SEV(log.lg, info) << "Info: %s (%s:%u, %s)\n" << localMsg.constData() << context.file << context.line << context.function;
        break;
    case QtWarningMsg:
        BOOST_LOG_SEV(log.lg, warning) << "Warning: %s (%s:%u, %s)\n" << localMsg.constData() << context.file << context.line << context.function;
        break;
    case QtCriticalMsg:
        BOOST_LOG_SEV(log.lg, error) << "Critical: %s (%s:%u, %s)\n" << localMsg.constData() << context.file << context.line << context.function;
        break;
    case QtFatalMsg:
        BOOST_LOG_SEV(log.lg, fatal) << "Fatal: %s (%s:%u, %s)\n" << localMsg.constData() << context.file << context.line << context.function;
        abort();
    }
    (*QT_DEFAULT_MESSAGE_HANDLER)(type, context, msg);
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	keymanager_dialog manager;
	manager.init();
	manager.show();

    //set logging redirection function
    qInstallMessageHandler(HandleLoggingOutput);

	a.exec();
	return 0;
}
