#ifndef BITCOINWIDGET_H
#define BITCOINWIDGET_H

#include <QWidget>
#include "KeychainWidget.h"
#include "SecureWindowElement.h"
#include "Transaction.h"
#include "ExpertModeElement.h"
#include <QVBoxLayout>
#include <QLabel>

class BitcoinWidget : public KeychainWidget
{
	Q_OBJECT
public:
	BitcoinWidget(Transaction& trnsaction, QWidget *parent = Q_NULLPTR);
		void SetPosition(int x, int y, int width) override;
	int GetCurrentHeight() override;
	int GetCurrentWidth() override;
	~BitcoinWidget();

private:
	SecureWindowElement * cryptoType = Q_NULLPTR;
	//SecureWindowElement **from;
	//SecureWindowElement ** to;
	//SecureWindowElement ** amount;
	SecureWindowElement * from;
	SecureWindowElement * to;
	SecureWindowElement * amount;
	ExpertModeElement * expertModeElement = Q_NULLPTR;
	int currentHeight = 0;
	int currentWidth = 0;
	int inElementCount = 0;
	int outElementCount = 0;
	int num_vins = 2;
	int num_vouts = 2;
};

#endif
