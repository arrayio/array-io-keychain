#ifndef ETHEREUMWIDGET_H
#define ETHEREUMWIDGET_H

#include <QWidget>
#include "Transaction.h"
#include "SecureWindowElement.h"
#include "ExpertModeElement.h"
#include "PrivateKeyInMemoryWidget.h"
#include "KeychainWidget.h"
#include "ui_keychain_gui_win.h"

namespace keychain_app
{

namespace secmod_commands
{
  class secmod_parser_f;
}
}

class LockIcon;
class PopupWindow;
class PasswordEnterElement;


class EthereumWidget : public QDialog
{
	Q_OBJECT
public:
	EthereumWidget(const secmod_commands::secmod_parser_f& parser, QWidget * parent = nullptr);
	virtual ~EthereumWidget();

private:
  static constexpr int FIELD_WIDTH = 446;
  static constexpr int START_POSITION = 96;
  Ui::keychain_gui_winClass ui;
  LockIcon * descriptionLabel = nullptr;
  PopupWindow * popupWindow = nullptr;
  QLabel * descriptionLabel = nullptr;
  PasswordEnterElement * password = nullptr;
	SecureWindowElement * cryptoType = nullptr;
	SecureWindowElement * from = nullptr;
	SecureWindowElement * to = nullptr;
	SecureWindowElement * amount = nullptr;
	ExpertModeElement * expertModeElement = nullptr;
	PrivateKeyInMemory * unlockTime= nullptr;
	int currentHeight = 0;
	int currentWidth = 0;
};

#endif

