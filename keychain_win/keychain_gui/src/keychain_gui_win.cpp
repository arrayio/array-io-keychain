#include "keychain_gui_win.h"

keychain_gui_win::keychain_gui_win(Transaction &transaction, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint);
	setFixedSize(600, 347);

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
		warningMessage.SetWarning(KeychainWarningMessage::WarningType::HexWarning);
		element = new UnlockKeyWidget(transaction, this);
		element->move(0, endControlPosition);
		element->SetPosition(0, endControlPosition, FIELD_WIDTH);
		endControlPosition += 10;
		endControlPosition = endControlPosition + element->GetCurrentHeight();
		descriptionLabel->setText("Unlock key with name <b>''" + transaction.unlockKeyName() + "''</b>. Are you sure you want to unlock?");
	}
	if (transaction.isCreatePassword()) {
		descriptionLabel->setStyleSheet("font:14px \"Segoe UI\";background:transparent;");
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
		}
		else {
			if (cmd_parse.unlock_time() > 0) {
				warningMessage.SetWarning(KeychainWarningMessage::WarningType::HexWarning);
			}
			switch (cmd_type)
			{
			case keychain_app::secmod_commands::blockchain_secmod_te::ethereum: {
				element = new EthereumWidget(transaction, this);
				break;
			}
			case keychain_app::secmod_commands::blockchain_secmod_te::ethereum_swap: {
				element = new EthereumSwapWidget(transaction, this);
				break;
			}
			case keychain_app::secmod_commands::blockchain_secmod_te::bitcoin:
			{
				element = new BitcoinWidget(transaction, this);
				break;
			}
			case keychain_app::secmod_commands::blockchain_secmod_te::rawhash:
			{
				warningMessage.SetWarning(KeychainWarningMessage::WarningType::HashWarnig);
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
	if (element != Q_NULLPTR)
		CancelButton->move(element->GetCurrentWidth() - 209, endControlPosition);
	else
		CancelButton->move(width() - 209, endControlPosition);

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

	setFixedHeight(endControlPosition + OKButton->height() + 15);
	if (element != Q_NULLPTR) {
		OKButton->move(element->GetCurrentWidth() - 112, endControlPosition);
		setFixedWidth(element->GetCurrentWidth() + 20);
		descriptionLabel->move(element->GetCurrentWidth() - 420, 0);
	}
	else {
		OKButton->move(width() - 112, endControlPosition);
		descriptionLabel->move(width() - 420, 0);
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
	this->connect(OKButton, &QPushButton::released, this, &keychain_gui_win::transaction_sign);
	this->connect(CancelButton, &QPushButton::released, this, &keychain_gui_win::cancel_sign);
	password->setValueFocus();
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
	serviceExchange->EncodeError(L"empty_password", 14);
	this->close();
}

void keychain_gui_win::show_transaction()
{

}

