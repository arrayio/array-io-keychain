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
	
	num_vouts = bitcoin_data.num_vouts;//chech num of vouts
	bool overflow = false;
	if (num_vouts > 15) {
		num_vouts = 15;
		overflow = true;
	}

	if (bitcoin_data.num_vouts >= 1) {
		//auto vout1 = bitcoin_data.vouts[0];
		from = new SecureWindowElement(this);
		from->SetLabelStyle(labelStyle);
		from->SetValueStyle(valueStyle);
		from->SetLabelAndValue("From", QString::fromStdString(bitcoin_trx.from));

		to = new SecureWindowElement*[num_vouts];
		amount = new SecureWindowElement*[num_vouts];
		
		for (int i = 0; i < num_vouts; i++) {
			auto vout1 = bitcoin_data.vouts[i];
			*(to + i) = new SecureWindowElement(this);
			(*(to + i))->SetLabelStyle(labelStyle);
			(*(to + i))->SetValueStyle(valueStyle);
			(*(to + i))->SetLabelAndValue("To", QString::fromStdString(vout1.address));

			*(amount+i) = new SecureWindowElement(this);
			(*(amount + i))->SetLabelStyle(labelStyle);
			(*(amount + i))->SetValueStyle(valueStyle);
			(*(amount + i))->SetLabelAndValue("Amount", QString::number(vout1.amount) + " BTC");
		}
	}

	if (cmd_parse.unlock_time() > 0) {
		unlockTime = new PrivateKeyInMemory(this);

		unlockTime->SetTime(QString::number(cmd_parse.unlock_time()));
	}

	if (overflow) {
		lookAll = new QPushButton(this);
		lookAll->setFixedSize(13, 7);
		lookAll->setFlat(true);
		lookAll->setStyleSheet("background-image:url(:/keychain_gui_win/more_arrow.png);border-style:outset;border-width:0px;");
		lookAll->setWindowFlags(Qt::FramelessWindowHint);
		lookAll->setCursor(Qt::PointingHandCursor);
		connect(lookAll, &QPushButton::clicked, this, &BitcoinWidget::lookUpAll);
	}
	expertModeElement = new ExpertModeElement(this);
	expertModeElement->SetExpertModeText(QString::fromStdString(cmd_parse.to_expert_mode_string()));
}

void BitcoinWidget::SetPosition(int x, int y, int width)
{
	cryptoType->SetPosition(0, 0, 116, width);
	cryptoType->SetLabelOffset(97);
	currentHeight = 26;
	from->SetPosition(0, currentHeight, 116, width);
	from->move(0, currentHeight);
	currentHeight += 26;
	
	for (int i = 0; i < num_vouts; i++) {
		(*(to+i))->SetPosition(0, currentHeight, 116, width*0.75);
		(*(to+i))->move(0, currentHeight);
		(*(amount+i))->SetPosition((*(to+i))->width(), currentHeight, 116, width / 3);
		(*(amount+i))->move((*(to+i))->width(), currentHeight);
		currentWidth = (*(to+i))->width() + (*(amount+i))->width();
		currentHeight += 26;
	}
	if (lookAll != Q_NULLPTR) {
		lookAll->move(currentWidth-40, currentHeight-10);
	}
	if (unlockTime != Q_NULLPTR) {
		unlockTime->SetPosition(0, currentHeight, 116, width);
		unlockTime->move(0, currentHeight);
		currentHeight += 36;
	}
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

void BitcoinWidget::lookUpAll() {

}

BitcoinWidget::~BitcoinWidget()
{

}