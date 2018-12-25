#include "keychain_gui_win.h"

//#include "widget.hpp"
#include <QApplication>
#include <QDesktopWidget>
#include <stdlib.h>
#include <iostream>

Q_DECLARE_METATYPE(std::string)

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    Widget w;

    QString srcTrans;
    Transaction trans(srcTrans);

    keychain_gui_win w(trans);

    const QRect sz = QApplication::desktop()->availableGeometry(&w);
    w.resize(sz.width() / 6, sz.height() * 1 / 6);
    w.move((sz.width() - w.width()) / 2,
                (sz.height() - w.height()) / 2);

    w.show();
   return a.exec();
}



