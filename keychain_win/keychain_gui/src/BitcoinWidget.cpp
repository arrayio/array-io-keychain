#include "BitcoinWidget.h"


BitcoinWidget::BitcoinWidget(const Transaction &transaction, QWidget * parent)
	:KeychainWidget(parent)
{
	QMetaObject::connectSlotsByName(this);

	QString labelStyle("font:16px \"Segoe UI\";background:transparent;");
	QString valueStyle("font:16px \"Segoe UI\";background:transparent;color:rgb(123,141,167)");

	cryptoType = new SecureWindowElement(this);
	cryptoType->SetLabelStyle("background-image:url(:/keychain_gui_win/btc_logo.png) no-repeat;background-size:contain;");
	cryptoType->SetValueStyle("font:16px \"Segoe UI\";background:transparent;color:rgb(123,141,167);");
	cryptoType->SetLabelAndValue("empty=Bitcoin");

	//QList<QString> fieldList({ "From","To","Amount" });

	
	/*inElementCount = 2;
	from = new SecureWindowElement*[num_vins];
	to = new SecureWindowElement*[num_vouts];*/

	//for (int i = 0; i < 2; i++) {
	//	*(from+i) =new SecureWindowElement(this);
	//	 (*(from+i))->setText("test label");
	//	(*(from + i))->setFixedSize(50, 25);
	//	(*(from+i))->SetLabelAndValue("From", "fdsfsdfsdfsdfsdfdffafdafasdf");

	//}
	from = new SecureWindowElement(this);
	from->SetLabelStyle(labelStyle);
	from->SetValueStyle(valueStyle);
	from->SetLabelAndValue("From", "fasfadafsdfsfafsdfasdfasdf");

	to = new SecureWindowElement(this);
	to->SetLabelStyle(labelStyle);
	to->SetValueStyle(valueStyle);
	to->SetLabelAndValue("To", "fsdfsdfsdfsdfsdfsdfsdfadfafasdf");

	amount = new SecureWindowElement(this);
	amount->SetLabelStyle(labelStyle);
	amount->SetValueStyle(valueStyle);
	amount->SetLabelAndValue("Amount", "0.2342342 BTC");

	expertModeElement = new ExpertModeElement(this);
	expertModeElement->SetExpertModeText(transaction.expertMode());

}

void BitcoinWidget::SetPosition(int x, int y, int width)
{
	cryptoType->SetPosition(0, 0, 116, width);
	cryptoType->SetLabelOffset(97);
	currentHeight = 26;
	//for (int i = 0; i < 2; i++) {
	//	(*(from+i))->move(0, currentHeight);
	//	currentHeight += 26;
	//}
	from->SetPosition(0, currentHeight, 116, width);
	currentHeight += 26;
	to->SetPosition(0, currentHeight, 116, width);
	amount->SetPosition(to->width(), currentHeight, 116, width);
	currentHeight += 26;
	expertModeElement->SetPosition(0, currentHeight, 116, width);
	currentHeight += 60;
	setFixedHeight(currentHeight);
}

int BitcoinWidget::GetCurrentHeight() {
	return currentHeight;
}

BitcoinWidget::~BitcoinWidget()
{

}