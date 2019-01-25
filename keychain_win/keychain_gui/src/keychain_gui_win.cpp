#include "keychain_gui_win.h"

keychain_gui_win::keychain_gui_win(QWidget *parent)
	: QDialog(parent)
  , mEndControlPosition(START_POSITION)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint);
	setFixedSize(600, 347);
	QInputMethod inputMethod();
	

	headerBlock = new QLabel(this);
	logoLabel = new QLabel(this);
	QPixmap logo(":/keychain_gui_win/kch_logo.png");
	logo= logo.scaled(100, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	logoLabel->setStyleSheet("background:transparent;");
	logoLabel->setPixmap(logo);
	logoLabel->move(25, 8);
	headerBlock->setFixedHeight(68);
	headerBlock->setStyleSheet("background-color:rgb(255,255,255);");
	setStyleSheet("background-color:rgb(242,243,246)");

	serviceExchange = new KeychainServiceExchange();


	descriptionLabel = new QLabel(this);
	descriptionLabel->setStyleSheet("font:16px \"Segoe UI\";background:transparent;");
	descriptionLabel->setWordWrap(true);
	descriptionLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	descriptionLabel->setFixedSize(343, 68);


	int _x = 0, _y = 204, _labelWidth = 116;


	if (element != Q_NULLPTR)
		headerBlock->setFixedWidth(element->GetCurrentWidth() + 20);
	else
		headerBlock->setFixedWidth(width());
	
	CancelButton = new QPushButton("CANCEL", this);
  	
	CancelButton->setFixedSize(89, 25);
	CancelButton->setFlat(true);
	CancelButton->setCursor(Qt::PointingHandCursor);
	CancelButton->setWindowFlags(Qt::FramelessWindowHint);
	CancelButton->setStyleSheet("color:rgb(147,148,151);background-image: url(:/keychain_gui_win/but_cancel.png);border-style:outset;border-width:0px;border-radius:5px;font:16px \"Segoe UI\"");

	if (element != Q_NULLPTR) {
		setFixedWidth(element->GetCurrentWidth() + 20);
		descriptionLabel->move(element->GetCurrentWidth() - 420, 0);
	}
	else {
		descriptionLabel->move(width() - 420, 0);
	}
	CancelButton->move(OKButton->x() - 95, mEndControlPosition);
	if (languageLabel != Q_NULLPTR) {
		languageLabel->setFixedSize(30, 25);
		languageLabel->setStyleSheet("background-color:rgb(70,134,255);color:white;border-style:outset;border-width:0px;padding:4px 2px 6px 2px;border-radius:2px;font:16px \"Segoe UI\"");
		languageLabel->move(CancelButton->x() - 35, mEndControlPosition);
		QString lang = QGuiApplication::inputMethod()->locale().languageToString(QGuiApplication::inputMethod()->locale().language());
		languageLabel->setText(lang.mid(0, 2).toUpper());
	}
	//if (!transaction.isCreatePassword() && warningMessage.isWarn()) {
		lockIcon = new LockIcon(mWarningMessage, this);
		popupWindow = new PopupWindow(mWarningMessage, this);
		popupWindow->setVisible(false);
		lockIcon->setFixedSize(22, 22);
		lockIcon->setSourceDialog(popupWindow);
		if (element != Q_NULLPTR)
			lockIcon->move(element->GetCurrentWidth() - 25, 28);
		else
			lockIcon->move(width() - 55, 28);
		lockIcon->setMouseTracking(true);
	//}

	this->connect(OKButton, &QPushButton::released, this, &keychain_gui_win::transaction_sign);
	this->connect(CancelButton, &QPushButton::released, this, &keychain_gui_win::cancel_sign);
	_roundCorners();
	password->setValueFocus();
	connect(password, &PasswordEnterElement::finishEnterPassword, this, &keychain_gui_win::transaction_sign);
}

void keychain_gui_win::transaction_sign() {
	QString passPhrase("");
	passPhrase = password->GetValue();
	if (passPhrase.isEmpty()) {
		serviceExchange->EncodeError(L"empty_password", 14);
		return;
	}
	serviceExchange->EncodeSuccess(passPhrase.toStdWString(), passPhrase.length());
	this->close();
}

void keychain_gui_win::cancel_sign() {
	serviceExchange->EncodeCancel();
	this->close();
}

void keychain_gui_win::set_sign_focus()
{
	//OKButton->setStyleSheet("color:white;background-color:rgb(179,205,255);border-style:outset;border-width:0px;border-radius:5px;font:16px \"Segoe UI\"");
	
}

void keychain_gui_win::changeLocale()
{
	if (languageLabel != Q_NULLPTR) {
		QString lang = QGuiApplication::inputMethod()->locale().languageToString(QGuiApplication::inputMethod()->locale().language());
		languageLabel->setText(lang.mid(0, 2).toUpper());
	}
}


void keychain_gui_win::_roundCorners()
{
	int radius = 10;
	QRegion region(0, 0, width(), height(), QRegion::Rectangle);
 
    // top left
    QRegion round (0, 0, 2* radius, 2* radius, QRegion::Ellipse);
    QRegion corner(0, 0, radius, radius, QRegion::Rectangle);
    region = region.subtracted(corner.subtracted(round));
 
    // top right
    round = QRegion(width()-2* radius, 0, 2* radius, 2* radius, QRegion::Ellipse);
    corner = QRegion(width()- radius, 0, radius, radius, QRegion::Rectangle);
    region = region.subtracted(corner.subtracted(round));
 
    // bottom right
    round = QRegion(width()-2* radius, height()-2* radius, 2* radius, 2* radius, QRegion::Ellipse);
    corner = QRegion(width()- radius, height()- radius, radius, radius, QRegion::Rectangle);
    region = region.subtracted(corner.subtracted(round));
 
    // bottom left
    round = QRegion(0, height()-2* radius, 2* radius, 2* radius, QRegion::Ellipse);
    corner = QRegion(0, height()- radius, radius, radius, QRegion::Rectangle);
    region = region.subtracted(corner.subtracted(round));
 
    setMask(region);
}

void keychain_gui_win::_disableSignButton()
{
	if (!password->validConfirm()) {
		OKButton->setStyleSheet("color:white;background-color:rgb(147,184,255);border-style:outset;border-width:0px;border-radius:5px;font:16px \"Segoe UI\"");
		OKButton->setDisabled(true);
	}
	else {
		OKButton->setStyleSheet("color:white;background-color:rgb(70,134,255);border-style:outset;border-width:0px;border-radius:5px;font:16px \"Segoe UI\"");
		OKButton->setDisabled(false);
	}
}

void keychain_gui_win::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Escape) {
		serviceExchange->EncodeCancel();
		this->close();
	}
	if (event->key() == Qt::Key_Enter) {
		QString passPhrase("");
		passPhrase = password->GetValue();
		if (passPhrase.isEmpty()) {
			serviceExchange->EncodeError(L"empty_password", 14);
			return;
		}
		serviceExchange->EncodeSuccess(passPhrase.toStdWString(), passPhrase.length());
		this->close();
	}
}

void keychain_gui_win::closeEvent(QCloseEvent * event)
{
	serviceExchange->EncodeCancel();
	this->close();
}