#include "EthereumWidget.h"


EthereumWidget::EthereumWidget(const Transaction &transaction, QWidget * parent)
	:KeychainWidget(parent)
{
	QMetaObject::connectSlotsByName(this);
	cryptoType = new SecureWindowElement(this);

	QString valueStyle("font:16px \"Segoe UI\";background:transparent;color:rgb(123,141,167);");
	QString labelStyle("background-image:url(:/keychain_gui_win/bg_ephir.png) no-repeat;");
	cryptoType->SetLabelStyle(labelStyle);
	cryptoType->SetValueStyle(valueStyle);
	cryptoType->SetLabelAndValue("empty=ethereum99");

	//QList<QString> fieldList({ "From","To","Amount" });

	from = new SecureWindowElement(this);
	from->SetLabelStyle(labelStyle);
	from->SetValueStyle(valueStyle);
	from->SetLabelAndValue("From", transaction.getValue("from"));

	to = new SecureWindowElement(this);
	to->SetLabelStyle(labelStyle);
	to->SetValueStyle(valueStyle);
	to->SetLabelAndValue("To", transaction.getValue("to"));


	amount = new SecureWindowElement(this);
	amount->SetLabelStyle(labelStyle);
	amount->SetValueStyle(valueStyle);
	amount->SetLabelAndValue("Amount", transaction.getValue("value"));
	
	expertModeElement = new ExpertModeElement(this);
	expertModeElement->SetExpertModeText(transaction.expertMode());
	
}

void EthereumWidget::SetPosition(int x, int y, int width)
{
	cryptoType->SetPosition(0, 0, 116, width);
	cryptoType->SetLabelOffset(100);
	currentHeight = 26;
	from->SetPosition(0, currentHeight, 116, width);
	currentHeight += 26;
	to->SetPosition(0, currentHeight, 116, width);
	currentHeight += 26;
	amount->SetPosition(0, currentHeight, 116, width);
	currentHeight += 26;
	expertModeElement->SetPosition(0, currentHeight, 116, width);
	currentHeight += 60;
	setFixedHeight(currentHeight);
}

int EthereumWidget::GetCurrentHeight() {
	return currentHeight;
}

EthereumWidget::~EthereumWidget()
{

}