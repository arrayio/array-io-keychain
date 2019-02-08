#include "keymanager_dialog.hpp"
#include <QtWidgets/QApplication>
#include <QJsonDocument>
//#include "Transaction.h"
#include <QString>
#include <QtGlobal>
#include <cwchar>
#include <keychain_lib/keychain_logger.hpp>
#include <keychain_lib/secmod_parser_cmd.hpp>
#include <boost/format.hpp>

using namespace std;
using namespace keychain_app;

namespace attrs = boost::log::attributes;

using secmod_commands::secmod_parser_f;

static const QtMessageHandler QT_DEFAULT_MESSAGE_HANDLER = qInstallMessageHandler(0);

void HandleLoggingOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    auto log = logger_singleton::instance("key manager");
    switch (type) {
    case QtDebugMsg:
        BOOST_LOG_SEV(log.lg, debug) << boost::format("Debug:\n%1% %2% %3% %4%\n") % localMsg.constData() % context.file % context.line % context.function;
        break;
    case QtInfoMsg:
        BOOST_LOG_SEV(log.lg, info) << boost::format("Info:\n%1% %2% %3% %4%\n") % localMsg.constData() % context.file % context.line % context.function;
        break;
    case QtWarningMsg:
        BOOST_LOG_SEV(log.lg, warning) << boost::format("Warning:\n%1% %2% %3% %4%\n") % localMsg.constData() % context.file % context.line % context.function;
        break;
    case QtCriticalMsg:
        BOOST_LOG_SEV(log.lg, error) << boost::format("Critical error:\n%1% %2% %3% %4%\n") % localMsg.constData() % context.file % context.line % context.function;
        break;
    case QtFatalMsg:
        BOOST_LOG_SEV(log.lg, fatal) << boost::format("Fatal error:\n%1% %2% %3% %4%\n") % localMsg.constData() % context.file % context.line % context.function;
        abort();
    }
    (*QT_DEFAULT_MESSAGE_HANDLER)(type, context, msg);
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(HandleLoggingOutput);
    qDebug() << "Key manager started";

	QApplication a(argc, argv);
	keymanager_dialog manager;
  
	manager.init();
	manager.show();
  
	a.exec();
    qDebug() << "Key manager stopped";
	return 0;
}
