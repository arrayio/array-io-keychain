#ifndef UNPARSEDTRANSACTIONWIDGET_H
#define UNPARSEDTRANSACTIONWIDGET_H

#include <QWidget>
#include "KeychainWidget.h"
#include "ExpertModeElement.h"
#include "PrivateKeyInMemoryWidget.h"

class UnparsedTransactionWidget : public KeychainWidget {
	Q_OBJECT
public:
	UnparsedTransactionWidget(QWidget * parent = Q_NULLPTR);
	void SetPosition(int x, int y, int width) override;
	int GetCurrentHeight() override;
	int GetCurrentWidth() override;
	~UnparsedTransactionWidget();

private:
	ExpertModeElement * expertModeElement = Q_NULLPTR;
	PrivateKeyInMemory * unlockTime = Q_NULLPTR;
	int currentHeight = 0;
	int currentWidth = 0;
};

#endif
