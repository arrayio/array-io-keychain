#include "widget.h"
#include <QApplication>
#include <QDesktopWidget>
#include <stdlib.h>
#include <iostream>
#define MAX_EVENTS  10

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




