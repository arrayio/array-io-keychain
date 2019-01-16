#include "keychain_gui_win.h"
#include "cmd.hpp"

keychain_gui_win::keychain_gui_win(Transaction &transaction, QWidget *parent)
	: QDialog(parent)
{
    ui.setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setFixedSize(600, 347);
    passClearOnExit = true;
    send_msg  = true;
}

void keychain_gui_win::refresh(Transaction& transaction)
{

    KeychainWarningMessage warningMessage;

    headerBlock = new QLabel(this);
    headerBlock->setFixedHeight(68);
    headerBlock->setStyleSheet("background-color:rgb(255,255,255);background-image:url(:/keychain_gui_win/header.png);background-repeat:no-repeat;");
    setStyleSheet("background-color:rgb(242,243,246)");

    serviceExchange = new KeychainServiceExchange();


    descriptionLabel = new QLabel(this);
    descriptionLabel->setStyleSheet("font:12px \"Segoe UI\";background:transparent;");
    descriptionLabel->setWordWrap(true);
    descriptionLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    descriptionLabel->setFixedSize(343, 68);


    int _x = 0, _y = 204, _labelWidth = 116;

    int endControlPosition = START_POSITION;

    if (transaction.isUnlockKey() != -1) {
        warningMessage.SetWarning(KeychainWarningMessage::WarningType::UnlockWarning);
        element = new UnlockKeyWidget(transaction, this);
        element->move(0, endControlPosition);
        element->SetPosition(0, endControlPosition, FIELD_WIDTH);
        endControlPosition += 10;
        endControlPosition = endControlPosition + element->GetCurrentHeight();
        descriptionLabel->setText("Unlock key with name <b>''" + transaction.unlockKeyName() + "''</b>. Are you sure you want to unlock?");
    }
    if (transaction.isCreatePassword()) {
        descriptionLabel->setStyleSheet("font:14px \"Segoe UI\";background:transparent;");
        warningMessage.SetWarning(KeychainWarningMessage::WarningType::CreateWarning);
        descriptionLabel->setText("Please enter new password");
    }

    if (!transaction.isCreatePassword() && transaction.isUnlockKey() == -1)
    {
        secmod_parser_f cmd_parse;
        auto cmd_type = cmd_parse(transaction.getTransactionText().toStdString());
        QString descr("Some application requires a passphrase to sign transaction with keyname <b>''" + QString::fromStdString(cmd_parse.keyname()) + "''</b>. Are you sure you want to sign?");
        descriptionLabel->setText(descr);
        if (!cmd_parse.is_json()) {
            element = new UnparsedTransactionWidget(transaction, this);
            warningMessage.SetWarning(KeychainWarningMessage::WarningType::FailedWarning);
            if (cmd_parse.unlock_time() > 0) {
                warningMessage.SetWarning(KeychainWarningMessage::WarningType::UnlockWarning);
            }
        }
        else {
            if (cmd_parse.unlock_time() > 0) {
                warningMessage.SetWarning(KeychainWarningMessage::WarningType::UnlockWarning);
            }
            switch (cmd_type)
            {
                case keychain_app::secmod_commands::blockchain_secmod_te::ethereum: {
                    element = new EthereumWidget(transaction, this);
                    warningMessage.SetWarning(KeychainWarningMessage::WarningType::NoWarnig);
                    break;
                }
                case keychain_app::secmod_commands::blockchain_secmod_te::ethereum_swap: {
                    element = new EthereumSwapWidget(transaction, this);
                    warningMessage.SetWarning(KeychainWarningMessage::WarningType::NoWarnig);
                    break;
                }
                case keychain_app::secmod_commands::blockchain_secmod_te::bitcoin:
                {
                    element = new BitcoinWidget(transaction, this);
                    warningMessage.SetWarning(KeychainWarningMessage::WarningType::NoWarnig);
                    break;
                }
                case keychain_app::secmod_commands::blockchain_secmod_te::rawhash:
                {
                    warningMessage.SetWarning(KeychainWarningMessage::WarningType::HashWarnig);
                    element = new RawHashWidget(transaction, this);
                    break;
                }
                case keychain_app::secmod_commands::blockchain_secmod_te::unknown:
                case keychain_app::secmod_commands::blockchain_secmod_te::parse_error:
                {
                    warningMessage.SetWarning(KeychainWarningMessage::WarningType::FailedWarning);
                    element = new UnparsedTransactionWidget(transaction, this);
                    break;
                }
                    break;
            }
        }
        element->move(0, endControlPosition);
        element->SetPosition(0, endControlPosition, FIELD_WIDTH);
        endControlPosition += 10;
        endControlPosition = endControlPosition + element->GetCurrentHeight();
    }

    password = new PasswordEnterElement(transaction.isCreatePassword(), this);
    password->SetLabel("Passphrase");
    password->SetPosition(0, endControlPosition, FIELD_WIDTH);
    password->move(0, endControlPosition);
    endControlPosition += password->GetElementHeigth();
    endControlPosition += 10;
    if (element != Q_NULLPTR)
        headerBlock->setFixedWidth(element->GetCurrentWidth() + 20);
    else
        headerBlock->setFixedWidth(width());
    if (transaction.isCreatePassword()) {
        OKButton = new QPushButton("CREATE", this);
    }
    if (transaction.isUnlockKey() != -1) {
        OKButton = new QPushButton("UNLOCK", this);
    }
    if (transaction.isCreatePassword() == false && transaction.isUnlockKey() == -1)
    {
        OKButton = new QPushButton("SIGN", this);
    }
    CancelButton = new QPushButton("CANCEL", this);

    OKButton->setFixedSize(89, 25);
    OKButton->setFlat(true);
    OKButton->setCursor(Qt::PointingHandCursor);
    OKButton->setStyleSheet("color:white;background-color:rgb(70,134,255);border-style:outset;border-width:0px;border-radius:5px;font:16px \"Segoe UI\"");
    OKButton->setWindowFlags(Qt::FramelessWindowHint);


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
    this->connect(OKButton, &QPushButton::released, this, &keychain_gui_win::found_pass);
    this->connect(OKButton, &QPushButton::released, this, &keychain_gui_win::close);
    this->connect(CancelButton, &QPushButton::released, this, &keychain_gui_win::close);
    this->connect(password, &PasswordEnterElement::focus, this, &keychain_gui_win::setFocusByMouse);

    _roundCorners();
    password->setValueFocus();
    //connect(password, &PasswordEnterElement::finishEnterPassword, this, &keychain_gui_win::transaction_sign);
    //if (transaction.isCreatePassword())
      //  connect(password, &PasswordEnterElement::changePassword, this, &keychain_gui_win::_disableSignButton);
}


void keychain_gui_win::cancel_sign() {
	serviceExchange->EncodeCancel();
	this->close();
}

void keychain_gui_win::set_sign_focus()
{
	//OKButton->setStyleSheet("color:white;background-color:rgb(179,205,255);border-style:outset;border-width:0px;border-radius:5px;font:16px \"Segoe UI\"");
	
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
/*
void keychain_gui_win::closeEvent(QCloseEvent * event)
{

	serviceExchange->EncodeCancel();
	this->close();
}
*/

void keychain_gui_win::closeEvent(QCloseEvent *event)
{
    if (send_msg)
    {
        passClearOnExit ?
        send(fc_light::json::to_string(fc_light::variant( master::cmd<( master::cmds::cancel)>().base))) :
        send(fc_light::json::to_string(fc_light::variant( master::cmd<( master::cmds::ok)>().base)));
    }

    event->accept();
}


void keychain_gui_win::found_pass() {
    //QString passPhrase("");
    //passPhrase = password->GetValue();
    //if (passPhrase.isEmpty()) {
    //	serviceExchange->EncodeError(L"empty_password", 14);
    //	return;
    //}
    //serviceExchange->EncodeSuccess(passPhrase.toStdWString(), passPhrase.length());
    passClearOnExit = false;
    //this->close();
}

/*
void keychain_gui_win::send(std::string a)
{
    if ( write(STDIN_FILENO, a.c_str(), a.length() ) != a.length() )
        close();
}
*/

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

void keychain_gui_win::closeExpertMode()
{
    emit element->closeExpertMode();
}
