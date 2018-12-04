#include "keychain_gui_win.h"

keychain_gui_win::keychain_gui_win(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint);
	setFixedWidth(500);
	setFixedHeight(347);
	setStyleSheet("background-image: url(:/keychain_gui_win/background.png)");

	serviceExchange = new KeychainServiceExchange();

	OKButton = new QPushButton("SIGN", this);
	CancelButton = new QPushButton("CANCEL", this);

	OKButton->move(388, 301);
	OKButton->setFixedWidth(89);
	OKButton->setFixedHeight(25);
	OKButton->setFlat(true);
	//OKButton->setStyleSheet("background-image: url(:/keychain_gui_win/Resources/but_ok.png);padding:0px;margin:0px;font:12pt \"Segoe UI\";");
	OKButton->setStyleSheet("color:white;background-image: url(:/keychain_gui_win/but_ok.png);border-style:outset;border-width:0px;border-radius:5px;font:9pt \"Segoe UI\"");
	OKButton->setWindowFlags(Qt::FramelessWindowHint);
	CancelButton->move(291, 301);
	CancelButton->setFixedWidth(89);
	CancelButton->setFixedHeight(25);
	CancelButton->setFlat(true);
	CancelButton->setWindowFlags(Qt::FramelessWindowHint);
	//CancelButton->setStyleSheet("background-image: url(:/keychain_gui_win/Resources/but_cancel.png);padding:0px;margin:0px;font:12pt \"Segoe UI\";");
	CancelButton->setStyleSheet("color:rgb(147,148,151);background-image: url(:/keychain_gui_win/but_cancel.png);border-style:outset;border-width:0px;border-radius:5px;font:9pt \"Segoe UI\"");

	descriptionLabel = new QLabel(this);
	descriptionLabel->setStyleSheet("font:9pt \"Segoe UI\";background:transparent;");
	descriptionLabel->setText("<b>''CryptoKitties''</b> requires a passphrase to sign transaction<br> with keyname <b>''test_1''</b>. Are you sure you want to sign?");
	descriptionLabel->move(137, 25);

	expertModeLabel = new QLabel(this);
	expertModeLabel->setStyleSheet("font:12pt \"Segoe UI\";background:transparent;");
	expertModeLabel->setText("Expert mode");
	expertModeLabel->setFixedWidth(116);
	expertModeLabel->move(0, 204);
	expertModeLabel->setFrameStyle(QFrame::NoFrame);
	expertModeLabel->setAlignment(Qt::AlignBottom | Qt::AlignRight);

	expertModeLabelValue = new QLabel(this);
	expertModeLabelValue->setStyleSheet("font:10pt \"Segoe UI\";background-image:url(:/keychain_gui_win/expert.png);color:rgb(70,134,255);padding:5px;");
	expertModeLabelValue->setFixedWidth(346);
	expertModeLabelValue->setFixedHeight(50);
	expertModeLabelValue->setWordWrap(true);
	expertModeLabelValue->move(131, 204);
	expertModeLabelValue->setFrameStyle(QFrame::NoFrame);
	expertModeLabelValue->setAlignment(Qt::AlignBottom | Qt::AlignLeft);

	MoreButton = new QPushButton(this);
	MoreButton->move(451, 241);
	MoreButton->setFixedWidth(13);
	MoreButton->setFixedHeight(7);
	MoreButton->setFlat(true);
	MoreButton->setStyleSheet("background-image: url(:/keychain_gui_win/more_arrow.png);border-style:outset;border-width:0px;");
	MoreButton->setWindowFlags(Qt::FramelessWindowHint);

	passPhrase = new QLabel(this);
	passPhrase->setStyleSheet("font:12pt \"Segoe UI\";background:transparent;");
	passPhrase->setText("Passphrase");
	passPhrase->setFixedWidth(116);
	passPhrase->move(0, 274);
	passPhrase->setFrameStyle(QFrame::NoFrame);
	passPhrase->setAlignment(Qt::AlignBottom | Qt::AlignRight);

	passPhraseValue = new QLineEdit(this);
	passPhraseValue->setStyleSheet("font:12pt \"Segoe UI\";background-image:url(:/keychain_gui_win/bg_edit.png);border-style:outset;border-width:0px;");
	passPhraseValue->setText("");
	passPhraseValue->setFixedWidth(346);
	passPhraseValue->move(131, 274);
	passPhraseValue->setEchoMode(QLineEdit::Password);
	passPhraseValue->setAlignment(Qt::AlignBottom | Qt::AlignLeft);

	
	this->connect(OKButton, &QPushButton::released, this, &keychain_gui_win::transaction_sign);
	this->connect(CancelButton, &QPushButton::released, this, &keychain_gui_win::cancel_sign);
	this->connect(MoreButton, &QPushButton::released, this, &keychain_gui_win::more_transaction);
}

void keychain_gui_win::SetTransaction(const Transaction& transaction)
{
	if (transaction.blockchain() == "ethereum") {
		cryptoType = new SecureWindowElement(this);
		cryptoType->SetPosition(0, 96, 116, 400);
		cryptoType->SetLabelStyle("background-image:url(:/keychain_gui_win/bg_ephir.png) no-repeat;");
		cryptoType->SetValueStyle("font:12pt \"Segoe UI\";background:transparent;color:rgb(123,141,167);");
		cryptoType->SetLabelOffset(100);
		cryptoType->SetLabelAndValue("empty=ethereum");

		from = new SecureWindowElement(this);
		from->SetPosition(0, 122, 116, 400);
		from->SetLabelStyle("font:12pt \"Segoe UI\";background:transparent;");
		from->SetValueStyle("font:12pt \"Segoe UI\";background:transparent;color:rgb(123,141,167)");
		from->SetLabelAndValue("From", transaction.getValue("from"));

		to = new SecureWindowElement(this);
		to->SetPosition(0, 148, 116, 400);
		to->SetLabelStyle("font:12pt \"Segoe UI\";background:transparent;");
		to->SetValueStyle("font:12pt \"Segoe UI\";background:transparent;color:rgb(123,141,167)");
		to->SetLabelAndValue("To", transaction.getValue("to"));

		amount = new SecureWindowElement(this);
		amount->SetPosition(0, 175, 116, 400);
		amount->SetLabelStyle("font:12pt \"Segoe UI\";background:transparent;");
		amount->SetValueStyle("font:12pt \"Segoe UI\";background:transparent;color:rgb(123,141,167)");
		amount->SetLabelAndValue("Amount", transaction.getValue("value"));
		mExpertValue = transaction.expertMode();;
		QString resultString = transaction.expertMode();
		resultString = resultString.insert(35, '\n');
		resultString = resultString.mid(0, 70);
		expertModeLabelValue->setText(resultString);
		lockIcon = new LockIcon(this);

		popupWindow = new PopupWindow(this);
		popupWindow->setVisible(false);
		lockIcon->setFixedHeight(22);
		lockIcon->setFixedWidth(22);
		lockIcon->setSourceDialog(popupWindow);
		lockIcon->move(455, 28);
		lockIcon->setMouseTracking(true);
	}
}

void keychain_gui_win::transaction_sign() {
	QString passPhrase("");
	passPhrase= this->passPhraseValue->text();
	if (passPhrase.isEmpty()) {
		serviceExchange->EncodeError(L"empty_password", 14);
		return;
	}
	serviceExchange->EncodeSuccess(passPhrase.toStdWString(), passPhrase.length());
}

void keychain_gui_win::cancel_sign() {
	this->close();
}

void keychain_gui_win::more_transaction() {
	QMessageBox msgBox;
	msgBox.setWindowFlags(Qt::FramelessWindowHint);
	msgBox.setStyleSheet("font:12pt \"Segoe UI\";background-color:rgb(227,232,248);color:rgb(70,134,255);");
	msgBox.setText(mExpertValue);
	msgBox.exec();
}
