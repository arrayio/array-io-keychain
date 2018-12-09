#include "EthereumWidget.h"


EthereumWidget::EthereumWidget(const Transaction &transaction, QWidget * parent)
	:KeychainWidget(parent)
{
	QMetaObject::connectSlotsByName(this);
	cryptoType = new SecureWindowElement(this);
	cryptoType->SetLabelStyle("background-image:url(:/keychain_gui_win/bg_ephir.png) no-repeat;");
	cryptoType->SetValueStyle("font:16px \"Segoe UI\";background:transparent;color:rgb(123,141,167);");
	cryptoType->SetLabelAndValue("empty=ethereum99");

	//QList<QString> fieldList({ "From","To","Amount" });

	from = new SecureWindowElement(this);
	from->SetLabelStyle("font:16px \"Segoe UI\";background:transparent;");
	from->SetValueStyle("font:16px \"Segoe UI\";background:transparent;color:rgb(123,141,167)");
	from->SetLabelAndValue("From", transaction.getValue("from"));

	to = new SecureWindowElement(this);
	to->SetLabelStyle("font:16px \"Segoe UI\";background:transparent;");
	to->SetValueStyle("font:16px \"Segoe UI\";background:transparent;color:rgb(123,141,167)");
	to->SetLabelAndValue("To", transaction.getValue("to"));


	amount = new SecureWindowElement(this);
	amount->SetLabelStyle("font:16px \"Segoe UI\";background:transparent;");
	amount->SetValueStyle("font:16px \"Segoe UI\";background:transparent;color:rgb(123,141,167)");
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