#include "BitcoinWidget.h"


BitcoinWidget::BitcoinWidget(Transaction &transaction, QWidget * parent)
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
	secmod_parser_f cmd_parse;
	auto cmd_type = cmd_parse(transaction.getTransactionText().toStdString());

	auto bitcoin_trx = cmd_parse.to_bitcoin();
	auto bitcoin_data = bitcoin_trx.trx_info;
	
	auto num_vouts = bitcoin_data.num_vouts;//chech num of vouts
	
	if (bitcoin_data.num_vouts >= 1) {
		auto vout1 = bitcoin_data.vouts[0];
		from = new SecureWindowElement(this);
		from->SetLabelStyle(labelStyle);
		from->SetValueStyle(valueStyle);
		from->SetLabelAndValue("From", QString::fromStdString(bitcoin_trx.from));

		to = new SecureWindowElement(this);
		to->SetLabelStyle(labelStyle);
		to->SetValueStyle(valueStyle);
		to->SetLabelAndValue("To", QString::fromStdString(vout1.address));

		amount = new SecureWindowElement(this);
		amount->SetLabelStyle(labelStyle);
		amount->SetValueStyle(valueStyle);
		amount->SetLabelAndValue("Amount", QString::number(vout1.amount) + " BTC");
	}

	expertModeElement = new ExpertModeElement(this);
	expertModeElement->SetExpertModeText(transaction.getTransactionText());

}

void BitcoinWidget::SetPosition(int x, int y, int width)
{
	cryptoType->SetPosition(0, 0, 116, width);
	cryptoType->SetLabelOffset(97);
	currentHeight = 26;
	from->SetPosition(0, currentHeight, 116, width);
	from->move(0, currentHeight);
	currentHeight += 26;
	to->SetPosition(0, currentHeight, 116, width*0.75);
	to->move(0, currentHeight);
	amount->SetPosition(to->width(), currentHeight, 116, width/2);
	amount->move(to->width(), currentHeight);
	currentWidth = to->width() + amount->width();
	currentHeight += 26;
	expertModeElement->SetPosition(0, currentHeight, 116, width);
	expertModeElement->move(0, currentHeight);
	currentHeight += 60;
	setFixedWidth(currentWidth);
	setFixedHeight(currentHeight);
}

int BitcoinWidget::GetCurrentHeight() {
	return currentHeight;
}

int BitcoinWidget::GetCurrentWidth() {
	return currentWidth;
}

BitcoinWidget::~BitcoinWidget()
{

}