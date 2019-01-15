#ifndef KEYTABLEVIEW_H
#define KEYTABLEVIEW_H

#include <QTreeView>
#include <QWidget>
#include <QLabel>
#include <QTreeWidget>
#include <QFrame>
#include <QAbstractItemView>
#include "keys_table_header.hpp"

class keys_table_view : public QTreeWidget
{
	Q_OBJECT
public:
	keys_table_view(QWidget *parent = Q_NULLPTR);

};

#endif
