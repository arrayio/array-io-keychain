#include "keys_table_view.hpp"

keys_table_view::keys_table_view(QWidget *parent)
	: QTreeWidget(parent)
{
	QMetaObject::connectSlotsByName(this);
	setStyleSheet("border-width:0px;");
	
	setColumnCount(3);
	setColumnWidth(0, 355);
	setColumnWidth(1, 418);
	setColumnWidth(2, 267);
	
	key_table_header *header = new key_table_header(this);
	QSize size0(355,36);
	QSize size1(418, 36);
	QSize size2(267, 36);
	header->setSizeHint(0, size0);
	header->setSizeHint(0, size1);
	header->setSizeHint(0, size2);

	setHeaderItem(header);

	QList<QTreeWidgetItem *> items;

	for (int i = 0; i < 10; ++i) {
		QStringList _list;
		_list.push_back(QString("itemcol0: %1").arg(i));
		_list.push_back(QString("itemcol1: %1").arg(i));
		_list.push_back(QString("itemcol2: %1").arg(i));
		items.append(new QTreeWidgetItem(this, _list));
	}
	insertTopLevelItems(0, items);
}
