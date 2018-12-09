#pragma once

#include <QtWidgets/QDialog>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QPushButton>
#include "PopupWindow.h"
#include "LockIcon.h"
#include "SecureWindowElement.h"
#include "ExpertModeElement.h"
#include "KeychainServiceExchange.h"
#include "Transaction.h"
#include "ui_keychain_gui_win.h"
#include "EthereumWidget.h"
#include "EthereumSwapWidget.h"
#include "KeychainWidget.h"
#include "BitcoinWidget.h"
#include "PrivateKeyInMemoryWidget.h"

class keychain_gui_win : public QDialog
{
	Q_OBJECT

private:
	Ui::keychain_gui_winClass ui;

public:
	keychain_gui_win(const Transaction &transaction, QWidget *parent = Q_NULLPTR);
	PopupWindow * popupWindow;
	
private:
	QString mExpertValue;
	Transaction _transaction;
	
	QPushButton * OKButton = Q_NULLPTR;
	QPushButton * CancelButton = Q_NULLPTR;
	
	QLabel * headerBlock = Q_NULLPTR;
	QLabel * passPhrase = Q_NULLPTR;
	QLabel * descriptionLabel = Q_NULLPTR;
	QLineEdit* passPhraseValue = Q_NULLPTR;
	LockIcon * lockIcon = Q_NULLPTR;
	KeychainWidget * element = Q_NULLPTR;

	KeychainServiceExchange * serviceExchange =NULL;

private:
	const int FIELD_WIDTH = 446;
	const int START_POSITION = 96;

public slots:
	void transaction_sign();
	void cancel_sign();
	void show_transaction();
};
