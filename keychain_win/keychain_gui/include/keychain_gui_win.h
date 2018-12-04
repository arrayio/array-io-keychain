#pragma once

#include <QtWidgets/QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "PopupWindow.h"
#include "LockIcon.h"
#include "SecureWindowElement.h"
#include "KeychainServiceExchange.h"
#include "Transaction.h"
#include "ui_keychain_gui_win.h"

class keychain_gui_win : public QDialog
{
	Q_OBJECT

private:
	Ui::keychain_gui_winClass ui;

public:
	void SetTransaction(const Transaction& transaction);
	keychain_gui_win(QWidget *parent = Q_NULLPTR); 
	PopupWindow * popupWindow;
	
private:
	QString mExpertValue;
	QPushButton * OKButton;
	QPushButton* CancelButton;
	QPushButton* MoreButton;

	/*QLabel * fromLabel;
	QLabel * fromLabelValue;*/
	/*QLabel* toLabel;
	QLabel* toLabelValue;*/
	/*QLabel* amountLabel;
	QLabel* amountLabelValue;*/
	QLabel * expertModeLabel;
	QLabel * expertModeLabelValue;
	QLabel * passPhrase;
	QLabel * descriptionLabel;
	QLineEdit* passPhraseValue;
	LockIcon * lockIcon;
	SecureWindowElement * cryptoType;
	SecureWindowElement * from;
	SecureWindowElement * to;
	SecureWindowElement * amount;

	KeychainServiceExchange* serviceExchange;

public slots:
	void transaction_sign();
	void cancel_sign();
	void more_transaction();
};
