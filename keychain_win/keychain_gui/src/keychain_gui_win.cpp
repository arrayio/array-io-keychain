#include "keychain_gui_win.h"

keychain_gui_win::keychain_gui_win(const Transaction &transaction, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint);
	setFixedWidth(500);
	setFixedHeight(347);

	headerBlock = new QLabel(this);
	headerBlock->setFixedHeight(68);
	headerBlock->setFixedWidth(500);
	headerBlock->setStyleSheet("background-image:url(:/keychain_gui_win/header.png);");

	setStyleSheet("background-color:rgb(242,243,246)");

	serviceExchange = new KeychainServiceExchange();

	OKButton = new QPushButton("SIGN", this);
	CancelButton = new QPushButton("CANCEL", this);
	OKButton->move(388, 306);
	OKButton->setFixedWidth(89);
	OKButton->setFixedHeight(25);
	OKButton->setFlat(true);
	OKButton->setCursor(Qt::PointingHandCursor);
	OKButton->setStyleSheet("color:white;background-image: url(:/keychain_gui_win/but_ok.png);border-style:outset;border-width:0px;border-radius:5px;font:9pt \"Segoe UI\"");
	OKButton->setWindowFlags(Qt::FramelessWindowHint);

	CancelButton->move(291, 306);
	CancelButton->setFixedWidth(89);
	CancelButton->setFixedHeight(25);
	CancelButton->setFlat(true);
	CancelButton->setCursor(Qt::PointingHandCursor);
	CancelButton->setWindowFlags(Qt::FramelessWindowHint);
	CancelButton->setStyleSheet("color:rgb(147,148,151);background-image: url(:/keychain_gui_win/but_cancel.png);border-style:outset;border-width:0px;border-radius:5px;font:9pt \"Segoe UI\"");

	descriptionLabel = new QLabel(this);
	descriptionLabel->setStyleSheet("font:9pt \"Segoe UI\";background:transparent;");
	descriptionLabel->setText("<b>''CryptoKitties''</b> requires a passphrase to sign transaction<br> with keyname <b>''test_1''</b>. Are you sure you want to sign?");
	descriptionLabel->move(137, 25);
	
	expertLabel = new QLabel(this);
	int _x = 0, _y = 204, _labelWidth = 116;
	expertLabel->setStyleSheet("font:12pt \"Segoe UI\";background:transparent;");
	expertLabel->setText("Expert mode");
	expertLabel->move(0, 204);
	expertLabel->setFixedSize(116,20);
	expertLabel->setFrameStyle(QFrame::NoFrame);
	expertLabel->setAlignment(Qt::AlignBottom | Qt::AlignRight);

	expertValue = new QLabel(this);
	expertValue->setStyleSheet("background-color:rgb(227,232,248);border-style:outset;border-width:0px;border-radius:7px;font:10pt \"Segoe UI\";color:rgb(70,134,255);padding:5px;");
	//expertModeLabelValue->setStyleSheet("font:10pt \"Segoe UI\";background-image:url(:/keychain_gui_win/expert.png);color:rgb(70,134,255);padding:5px;");
	expertValue->setWordWrap(true);
	expertValue->move(131, 204);
	expertValue->setFixedSize(FIELD_WIDTH, 50);
	expertValue->setFrameStyle(QFrame::NoFrame);
	expertValue->setAlignment(Qt::AlignCenter | Qt::AlignLeft);
	QString transText = transaction.expertMode();
	transText = transText.insert(35, '\n');
	transText = transText.mid(0, 70);
	transText = transText.append('...');
	expertValue->setText(transText);


	if (transaction.blockchain() == "ethereum") {
		if (transaction.isSwap()) {
			//fields = new SecureWindowElement[8];
		}
		else {
			//fields = new SecureWindowElement[4];
			_createFieldsForEthereum(transaction);
		}

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
		lockIcon->move(455, 28);
		lockIcon->setMouseTracking(true);
	}

	passPhrase = new QLabel(this);
	passPhrase->setStyleSheet("font:12pt \"Segoe UI\";background:transparent;");
	passPhrase->setText("Passphrase");
	passPhrase->setFixedWidth(116);
	passPhrase->move(0, 274);
	passPhrase->setFrameStyle(QFrame::NoFrame);
	passPhrase->setAlignment(Qt::AlignBottom | Qt::AlignRight);

	passPhraseValue = new QLineEdit(this);
	passPhraseValue->setStyleSheet("font:10pt \"Segoe UI\";background-image:url(:/keychain_gui_win/bg_edit.png);border-style:outset;border-width:0px;");
	passPhraseValue->setText("");
	passPhraseValue->setFixedSize(FIELD_WIDTH, 25);
	passPhraseValue->move(131, 274);
	passPhraseValue->setEchoMode(QLineEdit::Password);
	passPhraseValue->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

	moreButton = new QPushButton(this);
	moreButton->setFixedSize(13, 7);
	moreButton->move(454, 242);
	moreButton->setFlat(true);
	moreButton->setStyleSheet("background-image:url(:/keychain_gui_win/more_arrow.png);border-style:outset;border-width:0px;");
	moreButton->setWindowFlags(Qt::FramelessWindowHint);
	moreButton->setCursor(Qt::PointingHandCursor);

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

void keychain_gui_win::_createFieldsForEthereum(const Transaction &transaction)
{

	cryptoType = new SecureWindowElement(this);
	cryptoType->SetPosition(0, 96, 116, FIELD_WIDTH);
	cryptoType->SetLabelStyle("background-image:url(:/keychain_gui_win/bg_ephir.png) no-repeat;");
	cryptoType->SetValueStyle("font:12pt \"Segoe UI\";background:transparent;color:rgb(123,141,167);");
	cryptoType->SetLabelOffset(100);
	cryptoType->SetLabelAndValue("empty=ethereum");

	//QList<QString> fieldList({ "From","To","Amount" });

	from = new SecureWindowElement(this);
	from->SetPosition(0, 122, 116, FIELD_WIDTH);
	from->SetLabelStyle("font:12pt \"Segoe UI\";background:transparent;");
	from->SetValueStyle("font:12pt \"Segoe UI\";background:transparent;color:rgb(123,141,167)");
	from->SetLabelAndValue("From", transaction.getValue("from"));

	to = new SecureWindowElement(this);
	to->SetPosition(0, 148, 116, FIELD_WIDTH);
	to->SetLabelStyle("font:12pt \"Segoe UI\";background:transparent;");
	to->SetValueStyle("font:12pt \"Segoe UI\";background:transparent;color:rgb(123,141,167)");
	to->SetLabelAndValue("To", transaction.getValue("to"));


	amount = new SecureWindowElement(this);
	amount->SetPosition(0, 175, 116, FIELD_WIDTH);
	amount->SetLabelStyle("font:12pt \"Segoe UI\";background:transparent;");
	amount->SetValueStyle("font:12pt \"Segoe UI\";background:transparent;color:rgb(123,141,167)");
	amount->SetLabelAndValue("Amount", transaction.getValue("value"));

}

void keychain_gui_win::cancel_sign() {
	this->close();
}


