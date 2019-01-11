#ifndef KEY_TABLE_HEADER_H
#define KEY_TABLE_HEADER_H

#include <QWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>

class key_table_header : public QTreeWidgetItem
{
	//Q_OBJECT

public:
	key_table_header(QTreeWidget *parent = Q_NULLPTR);
};

#endif
