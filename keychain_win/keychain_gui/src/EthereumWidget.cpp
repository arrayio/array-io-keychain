#include "EthereumWidget.h"
#include "LabelsFactory.h"
#include "KeychainWarningMessage.h"

using secmod_commands::secmod_parser_f;
namespace sm_cmd = secmod_commands;

EthereumWidget::EthereumWidget(const secmod_parser_f& cmd_parse, QWidget * parent)
	: QDialog(parent)
  , descriptionLabel(CreateLabel<Labels_te::DESCRIPTION>()(this))
  , cryptoType(new SecureWindowElement(this))
  , from(new SecureWindowElement(this))
  , to(new SecureWindowElement(this))
  , amount(new SecureWindowElement(this))
  , expertModeElement(new ExpertModeElement(this))
{
  int endControlPosition = START_POSITION;
  int _x = 0, _y = 204, _labelWidth = 116;

  ui.setupUi(this);
  setWindowFlags(Qt::FramelessWindowHint);
  setFixedSize(600, 347);
  setStyleSheet("background-color:rgb(242,243,246)");

	QMetaObject::connectSlotsByName(this);

	QString valueStyle("font:16px \"Segoe UI\";background:transparent;color:rgb(123,141,167);");
	QString labelStyle("font:16px \"Segoe UI\";background:transparent;");
	cryptoType->SetLabelStyle("background-image:url(:/keychain_gui_win/bg_ephir.png) no-repeat;background-size:contain;");
	cryptoType->SetValueStyle(valueStyle);
	cryptoType->SetLabelAndValue("empty=ethereum");

	//QList<QString> fieldList({ "From","To","Amount" });

  auto cmd = cmd_parse.params< sm_cmd::events_te::sign_hex >();
  auto& eth_trx = cmd.get_trx_view<secmod_commands::blockchain_secmod_te::ethereum>();

	from->SetLabelAndValue("From", QString::fromStdString(eth_trx.from));
	from->SetLabelStyle(labelStyle);
	from->SetValueStyle(valueStyle);

	auto eth_data = eth_trx.trx_info;

	to->SetLabelAndValue("To", QString::fromStdString(eth_data.to));
	to->SetLabelStyle(labelStyle);
	to->SetValueStyle(valueStyle);

	amount->SetLabelAndValue("Amount", QString::fromStdString(eth_data.value));
	amount->SetLabelStyle(labelStyle);
	amount->SetValueStyle(valueStyle);

	if (cmd.unlock_time > 0) {
		unlockTime = new PrivateKeyInMemory(this);
		unlockTime->SetTime(QString::number(cmd.unlock_time));
	}

  expertModeElement->SetExpertModeText(QString::fromStdString(to_expert_mode_string(cmd)));
  
  KeychainWarningMessage warningMessage;

  QString descr("Are you sure you want to sign this transaction with key <b>''" + QString::fromStdString(cmd_parse.keyname()) + "''</b>?");
  descriptionLabel->setText(descr);

  if (cmd.unlock_time > 0) {
    warningMessage.SetWarning(KeychainWarningMessage::WarningType::UnlockUseWarning);
  }
  warningMessage.SetWarning(KeychainWarningMessage::WarningType::NoWarnig);

  endControlPosition += 10;
  endControlPosition = endControlPosition + currentHeight;

}

/*
void EthereumWidget::SetPosition(int x, int y, int width)
{
	cryptoType->SetPosition(0, 0, 116, width);
	cryptoType->SetLabelOffset(100); 
	cryptoType->move(0, currentHeight);
	currentHeight = 26;
	from->SetPosition(0, currentHeight, 116, width);
	from->move(0, currentHeight);
	currentHeight += 26;
	to->SetPosition(0, currentHeight, 116, width);
	to->move(0, currentHeight);
	currentHeight += 26;
	amount->SetPosition(0, currentHeight, 116, width);
	amount->move(0, currentHeight);
	currentHeight += 26;
	if (unlockTime != Q_NULLPTR) {
		unlockTime->SetPosition(0, currentHeight, 116, width);
		unlockTime->move(0, currentHeight);
		currentHeight += 36;
	}
	expertModeElement->SetPosition(0, currentHeight, 116, width);
	expertModeElement->move(0, currentHeight);
	currentHeight += 60;
	currentWidth = from->width();
	currentWidth = from->width();
	setFixedWidth(currentWidth);
	setFixedHeight(currentHeight);
}

int EthereumWidget::GetCurrentHeight() {
	return currentHeight;
}

int EthereumWidget::GetCurrentWidth() {
	return currentWidth;
}
*/
EthereumWidget::~EthereumWidget()
{

}