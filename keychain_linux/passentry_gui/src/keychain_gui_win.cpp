#include "keychain_gui_win.h"
#include "cmd.hpp"
#include "widget_singleton.h"

keychain_gui_win::keychain_gui_win(QWidget *parent)
	: QDialog(parent), close_event(false)
{
    ui.setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setFixedSize(600, 347);
}

void keychain_gui_win::refresh()
{
    KeychainWarningMessage warningMessage;

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
    descriptionLabel->setStyleSheet("font:12px \"Segoe UI\";background:transparent;");
    descriptionLabel->setWordWrap(true);
    descriptionLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    descriptionLabel->setFixedSize(343, 68);


    int _x = 0, _y = 204, _labelWidth = 116;

    int endControlPosition = START_POSITION;

	namespace sm_cmd = keychain_app::secmod_commands;
	auto event_num = shared_event::event_num();

	auto info_unlock = [&](std::string& keyname, int time){
		warningMessage.SetWarning(KeychainWarningMessage::WarningType::UnlockWarning);
		descriptionLabel->setText("You are trying to unlock the key \"" + QString::fromStdString(keyname) +
		"\" for \"" + QString::number(time) +"\" seconds");
	};

    auto info = [&](std::string & keyname) {
        QString descr("Are you sure you want to sign this transaction with key <b>''" + QString::fromStdString(keyname)
        + "''</b>?");
        descriptionLabel->setText(descr);
    };

	auto element_move = [&]{
		element->move(0, endControlPosition);
		element->SetPosition(0, endControlPosition, FIELD_WIDTH);
		endControlPosition += 10;
		endControlPosition = endControlPosition + element->GetCurrentHeight();
	};

	switch(event_num)
	{
		case(sm_cmd::events_te::create_key):
		{
			auto event = shared_event::ptr<sm_cmd::events_te::create_key>();
			warningMessage.SetWarning(KeychainWarningMessage::WarningType::CreateWarning);
			descriptionLabel->setText("Enter the password for the new key");
            OKButton = new QPushButton("CREATE", this);
			break;
		}
		case(sm_cmd::events_te::sign_hex):
		{
			auto event = shared_event::ptr<sm_cmd::events_te::sign_hex>();
	        if (event.get()->unlock_time)
                info_unlock(event.get()->keyname, event.get()->unlock_time);
	        else
                info(event.get()->keyname);

			if (event.get()->is_parsed) {
				switch (event.get()->blockchain) {
					case sm_cmd::blockchain_secmod_te::ethereum: {
						element = new EthereumWidget(this);
						warningMessage.SetWarning(KeychainWarningMessage::WarningType::NoWarnig);
						element_move();
						break;
					}
					case sm_cmd::blockchain_secmod_te::ethereum_swap: {
						element = new EthereumSwapWidget(this);
						warningMessage.SetWarning(KeychainWarningMessage::WarningType::NoWarnig);
						languageLabel = new QLabel(this);
                        element_move();
						break;
					}
					case sm_cmd::blockchain_secmod_te::bitcoin: {
						element = new BitcoinWidget(this);
						warningMessage.SetWarning(KeychainWarningMessage::WarningType::NoWarnig);
                        element_move();
						break;
					}
				}
			}
			else {
                element = new UnparsedTransactionWidget(this);
                element_move();
                warningMessage.SetWarning(KeychainWarningMessage::WarningType::FailedWarning);
			}
			OKButton = new QPushButton("SIGN", this);
			break;
		}
		case(sm_cmd::events_te::sign_hash):
		{
			auto event = shared_event::ptr<sm_cmd::events_te::sign_hash>();
            info(event.get()->keyname);
            element = new UnparsedTransactionWidget(this);
            warningMessage.SetWarning(KeychainWarningMessage::WarningType::FailedWarning);
            element_move();
            break;
		}
		case(sm_cmd::events_te::unlock):
		{
			auto event = shared_event::ptr<sm_cmd::events_te::unlock>();
            info_unlock(event.get()->keyname, event.get()->unlock_time);
			OKButton = new QPushButton("UNLOCK", this);
			break;
		}
	}

	password = new PasswordEnterElement(this);
	password->SetLabel("Passphrase");
	password->SetPosition(0, endControlPosition, FIELD_WIDTH);
	password->move(0, endControlPosition);
	endControlPosition += password->GetElementHeigth();
	endControlPosition += 10;

	if (element != Q_NULLPTR)
		headerBlock->setFixedWidth(element->GetCurrentWidth() + 20);
	else
		headerBlock->setFixedWidth(width());

	OKButton->setFixedSize(89, 25);
	OKButton->setFlat(true);
	OKButton->setCursor(Qt::PointingHandCursor);
	OKButton->setStyleSheet("color:white;background-color:rgb(70,134,255);border-style:outset;border-width:0px;border-radius:5px;font:16px \"Segoe UI\"");
	OKButton->setWindowFlags(Qt::FramelessWindowHint);

	CancelButton = new QPushButton("CANCEL", this);
	CancelButton->setFixedSize(89, 25);
	CancelButton->setFlat(true);
	CancelButton->setCursor(Qt::PointingHandCursor);
	CancelButton->setWindowFlags(Qt::FramelessWindowHint);
	CancelButton->setStyleSheet("color:rgb(147,148,151);background-image: url(:/keychain_gui_win/but_cancel.png);border-style:outset;border-width:0px;border-radius:5px;font:16px \"Segoe UI\"");

	setFixedHeight(endControlPosition + OKButton->height() + 15);
	if (element != Q_NULLPTR) {
		setFixedWidth(element->GetCurrentWidth() + 20);
		descriptionLabel->move(element->GetCurrentWidth() - 420, 0);
	}
	else {
		descriptionLabel->move(width() - 420, 0);
	}
	OKButton->move(width() - 109, endControlPosition);
	CancelButton->move(OKButton->x() - 95, endControlPosition);
	if (languageLabel != Q_NULLPTR) {
		languageLabel->setFixedSize(30, 25);
		languageLabel->setStyleSheet("background-color:rgb(70,134,255);color:white;border-style:outset;border-width:0px;padding:4px 2px 6px 2px;border-radius:2px;font:16px \"Segoe UI\"");
		languageLabel->move(CancelButton->x() - 35, endControlPosition);
		QString lang = QGuiApplication::inputMethod()->locale().languageToString(QGuiApplication::inputMethod()->locale().language());
		languageLabel->setText(lang.mid(0, 2).toUpper());
	}

	lockIcon = new LockIcon(warningMessage, this);
	popupWindow = new PopupWindow(warningMessage, this);
	popupWindow->setVisible(false);
	lockIcon->setFixedSize(22, 22);
	lockIcon->setSourceDialog(popupWindow);
	if (element != Q_NULLPTR)
		lockIcon->move(element->GetCurrentWidth() - 25, 28);
	else
		lockIcon->move(width() - 55, 28);
	lockIcon->setMouseTracking(true);

    this->connect(OKButton, &QPushButton::released, this, &keychain_gui_win::OkButtonPress);
    this->connect(CancelButton, &QPushButton::released, this, &keychain_gui_win::CancelButtonPress);
    this->connect(password, &PasswordEnterElement::focus, this, &keychain_gui_win::setFocusByMouse);
	_roundCorners();
	password->setValueFocus();
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

void keychain_gui_win::closeEvent(QCloseEvent *event)
{
    if (close_event)
        event->accept();
    else
        send(fc_light::json::to_string(fc_light::variant( master::cmd<( master::cmds::cancel)>().base)));
}

void keychain_gui_win::OkButtonPress() {
    send(fc_light::json::to_string(fc_light::variant( master::cmd<( master::cmds::ok)>().base)));
}

void keychain_gui_win::CancelButtonPress() {
    send(fc_light::json::to_string(fc_light::variant( master::cmd<( master::cmds::cancel)>().base)));
}

void keychain_gui_win::setFocusByMouse(int line)
{
    send(fc_light::json::to_string(fc_light::variant(static_cast<master::cmd_base>(master::cmd<(master::cmds::focus)>(line)))));
}

void keychain_gui_win::setFocusByTabKey(int line_edit)
{
    line_edit ? password->valueConfirm->setFocus(): password->value->setFocus();
}

void keychain_gui_win::passentry(int len, int line_edit)
{
    if (line_edit)
    {
        password->valueConfirm->setText(QString(len, '*'));
        password->valueConfirm->setFocus();
    }
    else
    {
        password->value->setText(QString (len, '*'));
        password->value->setFocus();
    }
}

void keychain_gui_win:: closeExpertMode()
{
    emit element->closeExpertMode();
}
