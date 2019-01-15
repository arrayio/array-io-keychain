#ifndef KEYTABLEROW_H
#define KEYTABLEROW_H

#include <QTreeWidgetITem>
#include <QTreeWidget>
#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QString>

class key_table_row : public QTreeWidgetItem
{
	
public:
	key_table_row(QTreeWidget *parent = Q_NULLPTR);
	void setShortKeyInfo(QString keyName, QString descroption, QString date);
	
private:
	QLabel *keyInfo = Q_NULLPTR;
};

#endif
