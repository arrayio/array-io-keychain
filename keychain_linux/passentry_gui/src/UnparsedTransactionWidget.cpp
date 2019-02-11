#include "UnparsedTransactionWidget.h"


UnparsedTransactionWidget::UnparsedTransactionWidget(Transaction &transaction, QWidget * parent)
	:KeychainWidget(parent)
{
	QMetaObject::connectSlotsByName(this);

	QString valueStyle("font:16px \"Segoe UI\";background:transparent;color:rgb(123,141,167);");
	QString labelStyle("font:16px \"Segoe UI\";background:transparent;");

    namespace sm_cmd = keychain_app::secmod_commands;
    secmod_parser_f cmd_parse;
    auto cmd_type = cmd_parse(transaction.getTransactionText().toStdString());
    std::string rawtx;
    int unlock_time;
    switch (cmd_type)
    {
        case sm_cmd::events_te::sign_hex:
        {
            auto cmd = cmd_parse.params<sm_cmd::events_te::sign_hex>();
            rawtx = sm_cmd::to_expert_mode_string(cmd);
            unlock_time = cmd.unlock_time;
            break;
        }
    }

	expertModeElement = new ExpertModeElement(this);
	expertModeElement->SetExpertModeText(QString::fromStdString(rawtx));

	if (unlock_time > 0) {
		unlockTime = new PrivateKeyInMemory(this);
		unlockTime->SetTime(QString::number(unlock_time));
	}
}

void UnparsedTransactionWidget::SetPosition(int x, int y, int width)
{
	expertModeElement->SetPosition(0, currentHeight, 116, width);
	expertModeElement->move(0, currentHeight);
	currentHeight += 60; 
	if (unlockTime != Q_NULLPTR) {
		unlockTime->SetPosition(0, currentHeight, 116, width);
		unlockTime->move(0, currentHeight);
		currentHeight += 36;
	}
	currentWidth = expertModeElement->width();
	currentWidth = expertModeElement->width();
	setFixedWidth(currentWidth);
	setFixedHeight(currentHeight);
}

int UnparsedTransactionWidget::GetCurrentHeight() {
	return currentHeight;
}

int UnparsedTransactionWidget::GetCurrentWidth() {
	return currentWidth;
}

UnparsedTransactionWidget::~UnparsedTransactionWidget()
{

}