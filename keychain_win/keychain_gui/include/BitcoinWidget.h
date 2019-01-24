#ifndef BITCOINWIDGET_H
#define BITCOINWIDGET_H

#include <QWidget>
#include <QLabel>
#include "KeychainWidget.h"
#include "SecureWindowElement.h"
#include "Transaction.h"
#include "ExpertModeElement.h"
#include "PrivateKeyInMemoryWidget.h"
#include <QVBoxLayout>
#include <QLabel>

class BitcoinWidget : public KeychainWidget
{
	Q_OBJECT
public:
  using bitcoin_event = keychain_app::secmod_commands::transaction_view
    <keychain_app::secmod_commands::blockchain_secmod_te::bitcoin>::type;
	BitcoinWidget(const bitcoin_event& bitcoin_event, QWidget *parent = Q_NULLPTR);
		void SetPosition(int x, int y, int width) override;
	int GetCurrentHeight() override;
	int GetCurrentWidth() override;
	~BitcoinWidget();

private:
	SecureWindowElement * cryptoType = Q_NULLPTR;
	//SecureWindowElement **from;
	SecureWindowElement ** to;
	SecureWindowElement ** amount;
	SecureWindowElement * from;
	//SecureWindowElement * to;
	//SecureWindowElement * amount;
	PrivateKeyInMemory * unlockTime=Q_NULLPTR;
	ExpertModeElement * expertModeElement = Q_NULLPTR;
	QPushButton * lookAll = Q_NULLPTR;
	int currentHeight = 0;
	int currentWidth = 0;
	int inElementCount = 0;
	int outElementCount = 0;
	int num_vins = 0;
	int num_vouts = 0;

public slots:
	void lookUpAll();
};

#endif
