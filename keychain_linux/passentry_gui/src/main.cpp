#include "widget.hpp"
#include <QApplication>
#include <QDesktopWidget>
#include <stdlib.h>
#include <iostream>
#include "Transaction.h"
#include "keychain_gui_win.h"
#include <QtGlobal>
#include <keychain_lib/keychain_logger.hpp>


void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    auto log = logger_singletone::instance();

    QByteArray localMsg = msg.toLocal8Bit();

    BOOST_LOG_SEV(log.lg, info) << "Debug:" <<localMsg.constData()<< " " << context.file << " "
    << context.line << " " << context.function;
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qInstallMessageHandler(myMessageOutput);
 //   w.show();
    QString  value("");
    Transaction trans(value);
    keychain_gui_win g(trans);
    Widget w(g);


   return a.exec();

}



