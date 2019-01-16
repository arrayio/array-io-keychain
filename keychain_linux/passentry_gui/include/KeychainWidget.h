#ifndef KEYCHAINWIDGET_H
#define KEYCHAINWIDGET_H

#include <QWidget>
#include <keychain_lib/secmod_parser_cmd.hpp>

using namespace keychain_app;
using secmod_commands::secmod_parser_f;

class KeychainWidget: public QWidget
{
	Q_OBJECT
public:
	KeychainWidget(QWidget *parent = Q_NULLPTR) : QWidget(parent) {
		QMetaObject::connectSlotsByName(this);
	};
	virtual void SetPosition(int x, int y, int width)=0;
	virtual int GetCurrentHeight()=0;
	virtual int GetCurrentWidth() = 0;



signals:
    void closeExpertMode();
};

#endif