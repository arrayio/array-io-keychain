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
	QSize sizeDescr((355 + 418 + 267), 36);
	header->setSizeHint(0, size0);
	header->setSizeHint(0, size1);
	header->setSizeHint(0, size2);

	setHeaderItem(header);

	QList<QTreeWidgetItem *> items;

	QTreeWidgetItem *details = new QTreeWidgetItem(this);
	QLabel  *label = new QLabel("test", this);
	this->setItemWidget(details, 0, label);
	
	
	for (int i = 0; i < 10; i++) {
		
		//QStringList _list;

		//_list.push_back(QString("itemcol0: %1").arg(i));
		//_list.push_back(QString("itemcol1: %1").arg(i));
		//_list.push_back(QString("itemcol2: %1").arg(i));
		
		QTreeWidgetItem *row = new QTreeWidgetItem(this);
		QTreeWidgetItem *description = new QTreeWidgetItem(row);
		QLabel *label0 = new QLabel(QString("itemcol0: %1").arg(i));
		label0->setStyleSheet("background-color:red;");
		QLabel *label1 = new QLabel(QString("itemcol1: %1").arg(i));
		QLabel *label2 = new QLabel(QString("itemcol2: %1").arg(i));
		QLabel * descrLabel = new QLabel("key_description");
		row->setSizeHint(0, size0);
		row->setSizeHint(1, size1);
		row->setSizeHint(2, size2);
		//row->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicator);

		row->insertChild(0, description);
		description->setFirstColumnSpanned(true);
		description->setSizeHint(0, sizeDescr);
		this->setItemWidget(row, 0, label0);
		this->setItemWidget(row, 1, label1); 
		this->setItemWidget(row, 2, label2);
		this->setItemWidget(description, 0, descrLabel);


		items.append(row);
	}
	insertTopLevelItems(0, items);
}
