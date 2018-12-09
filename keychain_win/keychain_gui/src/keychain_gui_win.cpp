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
	headerBlock->setStyleSheet("background-color:rgb(255,255,255);background-image:url(:/keychain_gui_win/header.png);background-repeat:no-repeat;");

	setStyleSheet("background-color:rgb(242,243,246)");

	serviceExchange = new KeychainServiceExchange();

	
	descriptionLabel = new QLabel(this);
	descriptionLabel->setStyleSheet("font:12px \"Segoe UI\";background:transparent;");
	descriptionLabel->setText("<b>''CryptoKitties''</b> requires a passphrase to sign transaction<br> with keyname <b>''test_1''</b>. Are you sure you want to sign?");
	
	
	int _x = 0, _y = 204, _labelWidth = 116;

	int endControlPosition = 0;
	if (transaction.blockchain() == "ethereum") {
		if (transaction.isSwap()) {
			element = new BitcoinWidget(transaction, this);//new EthereumSwapWidget(transaction, this);
			
		}
		else {
			element = new EthereumWidget(transaction, this);
			
		}
		element->move(0, START_POSITION);
		element->SetPosition(0, START_POSITION, FIELD_WIDTH);
	}

	endControlPosition += 10;

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
	
	endControlPosition += 35;
	headerBlock->setFixedWidth(element->GetCurrentWidth());

	OKButton = new QPushButton("SIGN", this);
	CancelButton = new QPushButton("CANCEL", this);
	CancelButton->move(element->GetCurrentWidth() - 209, endControlPosition);

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

	OKButton->move(element->GetCurrentWidth() -112, endControlPosition);
	setFixedHeight(endControlPosition+OKButton->height() + 15);
	setFixedWidth(element->GetCurrentWidth());
	descriptionLabel->move(element->GetCurrentWidth() - 363, 25);

	lockIcon = new LockIcon(this);

	popupWindow = new PopupWindow(this);
	popupWindow->setVisible(false);
	lockIcon->setFixedSize(22, 22);
	lockIcon->setSourceDialog(popupWindow);
	lockIcon->move(element->GetCurrentWidth() -45, 28);
	lockIcon->setMouseTracking(true);
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

