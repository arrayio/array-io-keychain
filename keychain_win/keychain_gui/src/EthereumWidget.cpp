#include "EthereumWidget.h"


EthereumWidget::EthereumWidget(const EthereumWidget::ethereum_event& eth_event, QWidget * parent)
	:KeychainWidget(parent)
{
	QMetaObject::connectSlotsByName(this);
	cryptoType = new SecureWindowElement(this);

	QString valueStyle("font:16px \"Segoe UI\";background:transparent;color:rgb(123,141,167);");
	QString labelStyle("font:16px \"Segoe UI\";background:transparent;");
	cryptoType->SetLabelStyle("background-image:url(:/keychain_gui_win/bg_ephir.png) no-repeat;background-size:contain;");
	cryptoType->SetValueStyle(valueStyle);
	cryptoType->SetLabelAndValue("empty=ethereum");

	//QList<QString> fieldList({ "From","To","Amount" });

	from = new SecureWindowElement(this);
	from->SetLabelAndValue("From", QString::fromStdString(eth_event.from));
	from->SetLabelStyle(labelStyle);
	from->SetValueStyle(valueStyle);

	auto eth_data = eth_event.trx_info;

	to = new SecureWindowElement(this);
	to->SetLabelAndValue("To", QString::fromStdString(eth_data.to));
	to->SetLabelStyle(labelStyle);
	to->SetValueStyle(valueStyle);

	amount = new SecureWindowElement(this);
	amount->SetLabelAndValue("Amount", QString::fromStdString(eth_data.value));
	amount->SetLabelStyle(labelStyle);
	amount->SetValueStyle(valueStyle);

	if (cmd_parse.unlock_time() > 0) {
		unlockTime = new PrivateKeyInMemory(this);
		unlockTime->SetTime(QString::number(cmd_parse.unlock_time()));
	}

	expertModeElement = new ExpertModeElement(this);
	expertModeElement->SetExpertModeText(QString::fromStdString(cmd_parse.to_expert_mode_string()));
	
}

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

EthereumWidget::~EthereumWidget()
{

}