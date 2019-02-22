#include "widget.hpp"
#include <QApplication>
#include <QDesktopWidget>
#include <stdlib.h>
#include <iostream>
#include "keychain_gui_win.h"
#include <QtGlobal>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    keychain_gui_win g;
    Widget w(g);


   return a.exec();

}



