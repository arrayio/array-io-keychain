#include "key_table_row.hpp"

key_table_row::key_table_row(QTreeWidget *parent)
	: QTreeWidgetItem(parent)
{
	QMetaObject::connectSlotsByName(parent);
}

void key_table_row::setShortKeyInfo(QString keyName, QString descroption, QString date)
{
}
