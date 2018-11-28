#include "keychain_gui_win.h"

keychain_gui_win::keychain_gui_win(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint);
	setFixedWidth(500);
	setFixedHeight(347);
	setStyleSheet("background-image: url(:/keychain_gui_win/Resources/background.png)");

	OKButton = new QPushButton("SIGN", this);
	CancelButton = new QPushButton("CANCEL", this);


	OKButton->move(388, 301);
	OKButton->setFixedWidth(89);
	OKButton->setFixedHeight(25);
	OKButton->setFlat(true);
	//OKButton->setStyleSheet("background-image: url(:/keychain_gui_win/Resources/but_ok.png);padding:0px;margin:0px;font:12pt \"Segoe UI\";");
	OKButton->setStyleSheet("color:white;background-image: url(:/keychain_gui_win/Resources/but_ok.png);border-style:outset;border-width:0px;border-radius:5px;font:9pt \"Segoe UI\"");
	OKButton->setWindowFlags(Qt::FramelessWindowHint);
	CancelButton->move(291, 301);
	CancelButton->setFixedWidth(89);
	CancelButton->setFixedHeight(25);
	CancelButton->setFlat(true);
	CancelButton->setWindowFlags(Qt::FramelessWindowHint);
	//CancelButton->setStyleSheet("background-image: url(:/keychain_gui_win/Resources/but_cancel.png);padding:0px;margin:0px;font:12pt \"Segoe UI\";");
	CancelButton->setStyleSheet("color:rgb(147,148,151);background-image: url(:/keychain_gui_win/Resources/but_cancel.png);border-style:outset;border-width:0px;border-radius:5px;font:9pt \"Segoe UI\"");

	descriptionLabel = new QLabel(this);
	descriptionLabel->setStyleSheet("font:9pt \"Segoe UI\";background:transparent;");
	descriptionLabel->setText("<b>''CryptoKitties''</b> requires a passphrase to sign transaction<br> with keyname <b>''test_1''</b>. Are you sure you want to sign?");
	descriptionLabel->move(137, 25);

	cryptoType = new SecureWindowElement(this);
	cryptoType->SetPosition(0, 96, 116, 346);
	cryptoType->SetLabelStyle("background-image:url(:/keychain_gui_win/Resources/bg_ephir.png) no-repeat;");
	cryptoType->SetValueStyle("font:12pt \"Segoe UI\";background:transparent;color:rgb(123,141,167);");
	cryptoType->SetLabelOffset(100);
	cryptoType->SetLabelAndValue("empty=etherium");

	fromLabel = new QLabel(this);
	fromLabel->setText("From");
	fromLabel->setStyleSheet("font:12pt \"Segoe UI\";background:transparent;");
	fromLabel->setFixedWidth(116);
	fromLabel->move(0, 122);
	fromLabel->setFrameStyle(QFrame::NoFrame);
	fromLabel->setAlignment(Qt::AlignBottom | Qt::AlignRight);

	fromLabelValue = new QLabel(this);
	fromLabelValue->setText("bc1qfz492cv70zz8c6xqjpvufqtp9nlw4q65yzwexv");
	fromLabelValue->setStyleSheet("font:12pt \"Segoe UI\";background:transparent;color:rgb(123,141,167)");
	fromLabelValue->setFixedWidth(346);
	fromLabelValue->move(131, 122);
	fromLabelValue->setFrameStyle(QFrame::NoFrame);
	fromLabelValue->setAlignment(Qt::AlignBottom | Qt::AlignLeft);

	toLabel = new QLabel(this);
	toLabel->setStyleSheet("font:12pt \"Segoe UI\";background:transparent;");
	toLabel->setText("To");
	toLabel->setFixedWidth(116);
	toLabel->move(0, 148);
	toLabel->setFrameStyle(QFrame::NoFrame);
	toLabel->setAlignment(Qt::AlignBottom | Qt::AlignRight);

	toLabelValue = new QLabel(this);
	toLabelValue->setStyleSheet("font:12pt \"Segoe UI\";background:transparent;color:rgb(123,141,167)");
	toLabelValue->setText("12c6DSiU4Rq3P4ZxziKxzrL5LmMBrzjrJX");
	toLabelValue->setFixedWidth(346);
	toLabelValue->move(131, 148);
	toLabelValue->setFrameStyle(QFrame::NoFrame);
	toLabelValue->setAlignment(Qt::AlignBottom | Qt::AlignLeft);

	amountLabel = new QLabel(this);
	amountLabel->setStyleSheet("font:12pt \"Segoe UI\";background:transparent;");
	amountLabel->setText("Amount");
	amountLabel->setFixedWidth(116);
	amountLabel->move(0, 175);
	amountLabel->setFrameStyle(QFrame::NoFrame);
	amountLabel->setAlignment(Qt::AlignBottom | Qt::AlignRight);

	amountLabelValue = new QLabel(this);
	amountLabelValue->setStyleSheet("font:12pt \"Segoe UI\";background:transparent;color:rgb(123,141,167)");
	amountLabelValue->setText("0.03048852 BTC");
	amountLabelValue->setFixedWidth(346);
	amountLabelValue->move(131, 175);
	amountLabelValue->setFrameStyle(QFrame::NoFrame);
	amountLabelValue->setAlignment(Qt::AlignBottom | Qt::AlignLeft);

	expertModeLabel = new QLabel(this);
	expertModeLabel->setStyleSheet("font:12pt \"Segoe UI\";background:transparent;");
	expertModeLabel->setText("Expert mode");
	expertModeLabel->setFixedWidth(116);
	expertModeLabel->move(0, 204);
	expertModeLabel->setFrameStyle(QFrame::NoFrame);
	expertModeLabel->setAlignment(Qt::AlignBottom | Qt::AlignRight);

	expertModeLabelValue = new QLabel(this);
	expertModeLabelValue->setStyleSheet("font:12pt \"Segoe UI\";background:transparent;color:rgb(123,141,167)");
	expertModeLabelValue->setText("Expert mode");
	expertModeLabelValue->setFixedWidth(346);
	expertModeLabelValue->move(131, 204);
	expertModeLabelValue->setFrameStyle(QFrame::NoFrame);
	expertModeLabelValue->setAlignment(Qt::AlignBottom | Qt::AlignLeft);

	passPhrase = new QLabel(this);
	passPhrase->setStyleSheet("font:12pt \"Segoe UI\";background:transparent;");
	passPhrase->setText("Passphrase");
	passPhrase->setFixedWidth(116);
	passPhrase->move(0, 274);
	passPhrase->setFrameStyle(QFrame::NoFrame);
	passPhrase->setAlignment(Qt::AlignBottom | Qt::AlignRight);

	passPhraseValue = new QLineEdit(this);
	passPhraseValue->setStyleSheet("font:12pt \"Segoe UI\";background-image:url(:/keychain_gui_win/Resources/bg_edit.png);border-style:outset;border-width:0px;");
	passPhraseValue->setText("Passphrase");
	passPhraseValue->setFixedWidth(346);
	passPhraseValue->move(131, 274);
	passPhraseValue->setEchoMode(QLineEdit::Password);
	passPhraseValue->setAlignment(Qt::AlignBottom | Qt::AlignLeft);

	lockIcon = new LockIcon(this);

	popupWindow = new PopupWindow(this);
	popupWindow->setVisible(false);
	lockIcon->setFixedHeight(22);
	lockIcon->setFixedWidth(22);
	lockIcon->setSourceDialog(popupWindow);
	lockIcon->move(455, 28);
	lockIcon->setMouseTracking(true);
}
