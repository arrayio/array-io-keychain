#include "keychain_gui_win.h"

keychain_gui_win::keychain_gui_win(const Transaction &transaction, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint);
	setFixedSize(600, 347);

	_transaction = transaction;

	headerBlock = new QLabel(this);
	headerBlock->setFixedHeight(68);
	headerBlock->setFixedWidth(600);
	headerBlock->setStyleSheet("background-color:rgb(255,255,255);background-image:url(:/keychain_gui_win/header.png);background-repeat:no-repeat;");

	setStyleSheet("background-color:rgb(242,243,246)");

	serviceExchange = new KeychainServiceExchange();

	
	descriptionLabel = new QLabel(this);
	descriptionLabel->setStyleSheet("font:12px \"Segoe UI\";background:transparent;");
	descriptionLabel->setText("<b>''CryptoKitties''</b> requires a passphrase to sign transaction<br> with keyname <b>''test_1''</b>. Are you sure you want to sign?");
	descriptionLabel->move(237, 25);
	
	//expertLabel = new QLabel(this);
	int _x = 0, _y = 204, _labelWidth = 116;
	/*expertLabel->setStyleSheet("font:12px \"Segoe UI\";background:transparent;");
	expertLabel->setText("Expert mode");
	
	expertLabel->setFixedSize(116,20);
	expertLabel->setFrameStyle(QFrame::NoFrame);
	expertLabel->setAlignment(Qt::AlignBottom | Qt::AlignRight);*/

	//expertValue = new QLabel(this);
	//expertValue->setStyleSheet("background-color:rgb(227,232,248);border-style:outset;border-width:0px;border-radius:7px;font:12px \"Segoe UI\";color:rgb(70,134,255);padding:5px;");
	////expertModeLabelValue->setStyleSheet("font:10pt \"Segoe UI\";background-image:url(:/keychain_gui_win/expert.png);color:rgb(70,134,255);padding:5px;");
	//expertValue->setWordWrap(true);
	//expertValue->setFixedSize(FIELD_WIDTH, 50);
	//expertValue->setFrameStyle(QFrame::NoFrame);
	//expertValue->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	
	//expertValue->setText(transText);

	int endControlPosition = 0;
	if (transaction.blockchain() == "ethereum") {
		if (transaction.isSwap()) {
			//fields = new SecureWindowElement[8];
			//endControlPosition = _createFieldsForSwap(transaction, START_POSITION);
			//endControlPosition = _createFieldsForEthereum(transaction, endControlPosition);
			element = new BitcoinWidget(transaction, this);//new EthereumSwapWidget(transaction, this);
			
		}
		else {
			//fields = new SecureWindowElement[4];
			//endControlPosition = _createFieldsForEthereum(transaction, START_POSITION);
			element = new EthereumWidget(transaction, this);
			
		}
		element->move(0, START_POSITION);
		element->SetPosition(0, START_POSITION, FIELD_WIDTH);

		/*swapLogo= new SecureWindowElement(this);
		swapLogo->SetPosition(0, 96, 116, 400);
		swapLogo->SetLabelStyle("background-image:url(:/keychain_gui_win/swap_logo.png) no-repeat;");
		swapLogo->SetValueStyle("font:12pt \"Segoe UI\";background:transparent;color:rgb(123,141,167);");
		swapLogo->SetLabelOffset(100);
		swapLogo->SetLabelAndValue("empty=ethereum");*/

		/*mExpertValue = transaction.expertMode();
		expert_control->SetValue(transaction.expertMode());*/

		lockIcon = new LockIcon(this);

		popupWindow = new PopupWindow(this);
		popupWindow->setVisible(false);
		lockIcon->setFixedSize(22, 22);
		lockIcon->setSourceDialog(popupWindow);
		lockIcon->move(555, 28);
		lockIcon->setMouseTracking(true);
	}

	endControlPosition += 10;
	//expertModeElement = new ExpertModeElement(this);
	//expertModeElement->SetPosition(0, endControlPosition, 116, FIELD_WIDTH);
	//expertModeElement->SetExpertModeText(transaction.expertMode());
	//expertLabel->move(0, endControlPosition);
	//expertValue->move(131, endControlPosition);

	/*moreButton = new QPushButton(this);*/
	//moreButton->move(131+ FIELD_WIDTH-23, endControlPosition+50-13);

	endControlPosition = START_POSITION + element->GetCurrentHeight();

	passPhrase = new QLabel(this);
	passPhrase->setStyleSheet("font:16px \"Segoe UI\";background:transparent;");
	passPhrase->setText("Passphrase");
	passPhrase->setFixedWidth(116);
	passPhrase->setFixedHeight(25);
	passPhrase->move(0, endControlPosition);
	passPhrase->setFrameStyle(QFrame::NoFrame);
	passPhrase->setAlignment(Qt::AlignBottom | Qt::AlignRight);

	passPhraseValue = new QLineEdit(this);
	passPhraseValue->setStyleSheet("font:16px \"Segoe UI\";background-color:white;border-style:solid;border-width:1px;border-radius:4px;border-color:rgb(225,224,224);");
	passPhraseValue->setText("");
	passPhraseValue->setFixedSize(FIELD_WIDTH, 25);
	passPhraseValue->move(131, endControlPosition);
	passPhraseValue->setEchoMode(QLineEdit::Password);
	passPhraseValue->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	/*moreButton->setFixedSize(13, 7);
	moreButton->setFlat(true);
	moreButton->setStyleSheet("background-image:url(:/keychain_gui_win/more_arrow.png);border-style:outset;border-width:0px;");
	moreButton->setWindowFlags(Qt::FramelessWindowHint);
	moreButton->setCursor(Qt::PointingHandCursor);*/
	
	endControlPosition += 35;

	OKButton = new QPushButton("SIGN", this);
	CancelButton = new QPushButton("CANCEL", this);
	CancelButton->move(391, endControlPosition);

	OKButton->setFixedSize(89, 25);
	OKButton->setFlat(true);
	OKButton->setCursor(Qt::PointingHandCursor);
	OKButton->setStyleSheet("color:white;background-image: url(:/keychain_gui_win/but_ok.png);border-style:outset;border-width:0px;border-radius:5px;font:16px \"Segoe UI\"");
	OKButton->setWindowFlags(Qt::FramelessWindowHint);


	CancelButton->setFixedSize(89, 25);
	CancelButton->setFlat(true);
	CancelButton->setCursor(Qt::PointingHandCursor);
	CancelButton->setWindowFlags(Qt::FramelessWindowHint);
	CancelButton->setStyleSheet("color:rgb(147,148,151);background-image: url(:/keychain_gui_win/but_cancel.png);border-style:outset;border-width:0px;border-radius:5px;font:16px \"Segoe UI\"");

	OKButton->move(488, endControlPosition);
	setFixedHeight(endControlPosition+OKButton->height() + 15);
	//this->connect(moreButton, &QPushButton::released, this, &keychain_gui_win::show_transaction);
	this->connect(OKButton, &QPushButton::released, this, &keychain_gui_win::transaction_sign);
	this->connect(CancelButton, &QPushButton::released, this, &keychain_gui_win::cancel_sign);
}

void keychain_gui_win::transaction_sign() {
	QString passPhrase("");
	passPhrase = this->passPhraseValue->text();
	if (passPhrase.isEmpty()) {
		serviceExchange->EncodeError(L"empty_password", 14);
		return;
	}
	serviceExchange->EncodeSuccess(passPhrase.toStdWString(), passPhrase.length());
}

int keychain_gui_win::_createFieldsForEthereum(const Transaction &transaction, const int startPosition) //96
{

	//cryptoType = new SecureWindowElement(this);
	//cryptoType->SetPosition(0, startPosition, 116, FIELD_WIDTH);
	//cryptoType->SetLabelStyle("background-image:url(:/keychain_gui_win/bg_ephir.png) no-repeat;");
	//cryptoType->SetValueStyle("font:16px \"Segoe UI\";background:transparent;color:rgb(123,141,167);");
	//cryptoType->SetLabelOffset(100);
	//cryptoType->SetLabelAndValue("empty=ethereum");

	////QList<QString> fieldList({ "From","To","Amount" });

	//from = new SecureWindowElement(this);
	//from->SetPosition(0, startPosition+26, 116, FIELD_WIDTH);
	//from->SetLabelStyle("font:16px \"Segoe UI\";background:transparent;");
	//from->SetValueStyle("font:16px \"Segoe UI\";background:transparent;color:rgb(123,141,167)");
	//from->SetLabelAndValue("From", transaction.getValue("from"));

	//to = new SecureWindowElement(this);
	//to->SetPosition(0, startPosition+26*2, 116, FIELD_WIDTH);
	//to->SetLabelStyle("font:16px \"Segoe UI\";background:transparent;");
	//to->SetValueStyle("font:16px \"Segoe UI\";background:transparent;color:rgb(123,141,167)");
	//to->SetLabelAndValue("To", transaction.getValue("to"));


	//amount = new SecureWindowElement(this);
	//amount->SetPosition(0, startPosition+26*3, 116, FIELD_WIDTH);
	//amount->SetLabelStyle("font:16px \"Segoe UI\";background:transparent;");
	//amount->SetValueStyle("font:16px \"Segoe UI\";background:transparent;color:rgb(123,141,167)");
	//amount->SetLabelAndValue("Amount", transaction.getValue("value"));

	return (startPosition + 26 * 4);
}

int keychain_gui_win::_createFieldsForSwap(const Transaction &transaction, const int startPosition) //96
{

	/*swap = new SecureWindowElement(this);
	swap->SetPosition(0, startPosition, 116, FIELD_WIDTH);
	swap->SetLabelStyle("background-image:url(:/keychain_gui_win/swap_logo.png) no-repeat;");
	swap->SetValueStyle("font:14px \"Segoe UI\";background:transparent;color:rgb(123,141,167);");
	swap->SetLabelOffset(72);

	action = new SecureWindowElement(this);
	action->SetPosition(0, startPosition+26, 116, FIELD_WIDTH);
	action->SetLabelStyle("font:14px \"Segoe UI\";background:transparent;");
	action->SetValueStyle("font:14px \"Segoe UI\";background:transparent;color:rgb(123,141,167)");
	action->SetLabelAndValue("Action", transaction.getSwapValue("action"));

	hash = new SecureWindowElement(this);
	hash->SetPosition(0, startPosition+26*2, 116, FIELD_WIDTH);
	hash->SetLabelStyle("font:14px \"Segoe UI\";background:transparent;");
	hash->SetValueStyle("font:14px \"Segoe UI\";background:transparent;color:rgb(123,141,167)");
	hash->SetLabelAndValue("Hash", transaction.getSwapValue("hash"));


	address = new SecureWindowElement(this);
	address->SetPosition(0, startPosition+26*3, 116, FIELD_WIDTH);
	address->SetLabelStyle("font:14px \"Segoe UI\";background:transparent;");
	address->SetValueStyle("font:14px \"Segoe UI\";background:transparent;color:rgb(123,141,167)");
	address->SetLabelAndValue("Address", transaction.getSwapValue("address"));*/

	return (startPosition + 26 * 4);
}

void keychain_gui_win::cancel_sign() {
	this->close();
}

void keychain_gui_win::show_transaction()
{
	QMessageBox msgBox;
	msgBox.setText(_transaction.expertMode());
	msgBox.setWindowFlags(Qt::FramelessWindowHint);
	msgBox.setStyleSheet("background-color:rgb(227,232,248);border-style:outset;border-width:0px;border-radius:7px;font:11pt \"Segoe UI\";color:rgb(70,134,255);padding:5px;");
	msgBox.exec();
}
