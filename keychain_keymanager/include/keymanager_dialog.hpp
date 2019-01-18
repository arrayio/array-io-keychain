#ifndef  KEYMANAGERDIALOG_H
#define KEYMANAGERDIALOG_H

#include <QtWidgets/QDialog>
#include <QObject>
#include <QWidget>
#include <QApplication>
#include <QLabel>
#include <QTreeWidget>
#include <QFontDatabase>
#include <QFont>
#include "keys_table_view.hpp"
#include "menu_toolbar.hpp"
#include "key_scroll_area.hpp"
#include "keylist.hpp"

class keymanager_dialog : public QDialog
{
	Q_OBJECT
public:
	keymanager_dialog(QWidget *parent = Q_NULLPTR);

private:
	QLabel *header;
	QLabel *logoLabel;
	QLabel *logoTitle;
	key_scrooll_area *keys_list;
	keys_table_view *keys_table;
	QLabel *wList;
    menu_toolbar *toolbar;
};

#endif // ! KEYMANAGERDIALOG_H

