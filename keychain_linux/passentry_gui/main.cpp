#include "widget.hpp"
#include <QApplication>
#include <QDesktopWidget>
#include <stdlib.h>
#include <iostream>

Q_DECLARE_METATYPE(std::string)

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

    const QRect sz = QApplication::desktop()->availableGeometry(&w);
    w.resize(sz.width() / 4, sz.height() * 1 / 6);
    w.move((sz.width() - w.width()) / 2,
                (sz.height() - w.height()) / 2);

    w.show();
   return a.exec();
}




