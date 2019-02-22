#ifndef ETHEREUMWIDGET_H
#define ETHEREUMWIDGET_H

#include <QWidget>
#include "SecureWindowElement.h"
#include "ExpertModeElement.h"
#include "PrivateKeyInMemoryWidget.h"
#include "KeychainWidget.h"


class EthereumWidget : public KeychainWidget
{
	Q_OBJECT
public:
	EthereumWidget(QWidget * parent = Q_NULLPTR);
	void SetPosition(int x, int y, int width) override;
	int GetCurrentHeight() override;
	int GetCurrentWidth() override;
	~EthereumWidget();

private:
	SecureWindowElement * cryptoType = Q_NULLPTR;
	SecureWindowElement * from = Q_NULLPTR;
	SecureWindowElement * to = Q_NULLPTR;
	SecureWindowElement * amount = Q_NULLPTR;
	ExpertModeElement * expertModeElement = Q_NULLPTR;
	PrivateKeyInMemory * unlockTime= Q_NULLPTR;
	int currentHeight = 0;
	int currentWidth = 0;
};

#endif

