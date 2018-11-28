#include "keychain_gui_win.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	keychain_gui_win w;
	w.show();
	return a.exec();
}
