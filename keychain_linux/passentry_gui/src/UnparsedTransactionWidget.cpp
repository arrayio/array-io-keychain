#include "UnparsedTransactionWidget.h"
#include "widget_singleton.h"


UnparsedTransactionWidget::UnparsedTransactionWidget(Transaction &transaction, QWidget * parent)
	:KeychainWidget(parent)
{
	QMetaObject::connectSlotsByName(this);

	QString valueStyle("font:16px \"Segoe UI\";background:transparent;color:rgb(123,141,167);");
	QString labelStyle("font:16px \"Segoe UI\";background:transparent;");

	namespace sm_cmd = keychain_app::secmod_commands;
	using event_ptr = event_singleton<sm_cmd::secmod_event<sm_cmd::events_te::sign_hex>::params_t>;

	expertModeElement = new ExpertModeElement(this);
	expertModeElement->SetExpertModeText(QString::fromStdString(
			sm_cmd::to_expert_mode_string(*event_ptr::shared.get())
	));

	if (event_ptr::shared.get()->unlock_time > 0) {
		unlockTime = new PrivateKeyInMemory(this);
		unlockTime->SetTime(QString::number(event_ptr::shared.get()->unlock_time));
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