#pragma once

#include <QtWidgets/QDialog>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QInputMethod>
#include <QPushButton>
#include <QMessageBox>
#include <QPixmap>
#include "PopupWindow.h"
#include "LockIcon.h"
#include "SecureWindowElement.h"
#include "ExpertModeElement.h"
#include "KeychainServiceExchange.h"
#include "ui_keychain_gui_win.h"
#include "EthereumWidget.h"
#include "EthereumSwapWidget.h"
#include "KeychainWidget.h"
#include "BitcoinWidget.h"
#include "PrivateKeyInMemoryWidget.h"
#include <keychain_lib/secmod_parser_cmd.hpp>
#include "UnparsedTransactionWidget.h"
#include "PasswordEnterElement.h"
#include "UnlockKeyWidget.h"
#include "KeychainWarningMessage.h"
#include "RawHashWidget.h"

using namespace keychain_app;
using secmod_commands::secmod_parser_f;

class keychain_gui_win : public QDialog
{
	Q_OBJECT

private:
	Ui::keychain_gui_winClass ui;

public:

	keychain_gui_win(QWidget *parent = Q_NULLPTR);
	PopupWindow * popupWindow;
    void refresh();
    PasswordEnterElement * password;
	void setFocusByMouse(int);
    void setFocusByTabKey(int);
    void passentry (int, int);
    void closeExpertMode();
	bool close_event;

private:
	QString mExpertValue;
	QLabel * logoLabel;
	QPushButton * OKButton = Q_NULLPTR;
	QPushButton * CancelButton = Q_NULLPTR;

	QLabel * languageLabel = Q_NULLPTR;

	QLabel * headerBlock = Q_NULLPTR;
	/*QLabel * passPhrase = Q_NULLPTR;
	QLineEdit * passPhraseValue = Q_NULLPTR;*/
	//QLabel * message= Q_NULLPTR;
	QLabel * descriptionLabel = Q_NULLPTR;
	LockIcon * lockIcon = Q_NULLPTR;
	KeychainWidget * element = Q_NULLPTR;

	KeychainServiceExchange * serviceExchange =NULL;

private:
	void _roundCorners();
	void _disableSignButton();

protected:
	void keyPressEvent(QKeyEvent *event) override;
	void closeEvent(QCloseEvent * event) override;

private:
	const int FIELD_WIDTH = 446;
	const int START_POSITION = 96;

public slots:

	void OkButtonPress();
	void CancelButtonPress();
	void cancel_sign();
	void set_sign_focus();
	void changeLocale();
};
