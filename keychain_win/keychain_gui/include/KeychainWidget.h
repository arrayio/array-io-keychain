#ifndef KEYCHAINWIDGET_H
#define KEYCHAINWIDGET_H

#include <QWidget>

class KeychainWidget: public QWidget
{
	Q_OBJECT
public:
	KeychainWidget(QWidget *parent = Q_NULLPTR) : QWidget(parent) {
		QMetaObject::connectSlotsByName(this);
	};
	virtual void SetPosition(int x, int y, int width)=0;
	virtual int GetCurrentHeight()=0;
};

#endif