#include "widget.hpp"
#include <QApplication>
#include <QDesktopWidget>
#include <stdlib.h>
#include <iostream>
#include "Transaction.h"
#include "keychain_gui_win.h"
#include <QtGlobal>
#include <keychain_lib/keychain_logger.hpp>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString  value("");
    Transaction trans(value);
    keychain_gui_win g(trans);
    Widget w(g);


   return a.exec();

}



