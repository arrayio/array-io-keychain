#pragma once

#include <QtWidgets/QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "PopupWindow.h"
#include "LockIcon.h"
#include "SecureWindowElement.h"
#include "ui_keychain_gui_win.h"

class keychain_gui_win : public QDialog
{
	Q_OBJECT

public:
	keychain_gui_win(QWidget *parent = Q_NULLPTR); 
	PopupWindow * popupWindow;
protected:
	
private:
	QPushButton * OKButton;
	QPushButton* CancelButton;

	QLabel * fromLabel;
	QLabel * fromLabelValue;
	QLabel* toLabel;
	QLabel* toLabelValue;
	QLabel* amountLabel;
	QLabel* amountLabelValue;
	QLabel * expertModeLabel;
	QLabel * expertModeLabelValue;
	QLabel * passPhrase;
	QLabel * descriptionLabel;
	QLineEdit* passPhraseValue;
	LockIcon * lockIcon;
	SecureWindowElement * cryptoType;

private:
	Ui::keychain_gui_winClass ui;
};
