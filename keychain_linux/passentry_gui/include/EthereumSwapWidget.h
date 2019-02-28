#ifndef ETHEREUMSWAPWIDGET_H
#define ETHEREUMSWAPWIDGET_H

#include <QWidget>
#include "KeychainWidget.h"
#include "SecureWindowElement.h"
#include "PrivateKeyInMemoryWidget.h"
#include "ExpertModeElement.h"

class EthereumSwapWidget : public KeychainWidget
{
	Q_OBJECT
public:
	EthereumSwapWidget(QWidget * parent = Q_NULLPTR);
	void SetPosition(int x, int y, int width) override;
	int GetCurrentHeight() override;
	int GetCurrentWidth() override;
	~EthereumSwapWidget();

private:

	SecureWindowElement * swap = Q_NULLPTR;
	SecureWindowElement * action = Q_NULLPTR;
	SecureWindowElement * hash = Q_NULLPTR;
	SecureWindowElement * address = Q_NULLPTR;

	SecureWindowElement * cryptoType = Q_NULLPTR;
	SecureWindowElement * from = Q_NULLPTR;
	SecureWindowElement * to = Q_NULLPTR;
	SecureWindowElement * amount = Q_NULLPTR;
	PrivateKeyInMemory * unlockTime = Q_NULLPTR;
	ExpertModeElement * expertModeElement = Q_NULLPTR;
	int currentHeight = 0;
	int currentWidth = 0;
};
#endif // !ETHEREUMSWAPWIDGET_H

