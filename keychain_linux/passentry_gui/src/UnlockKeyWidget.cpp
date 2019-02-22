#include "UnlockKeyWidget.h"
#include "widget_singleton.h"

UnlockKeyWidget::UnlockKeyWidget(QWidget * parent)
	:KeychainWidget(parent)
{
	QMetaObject::connectSlotsByName(this);

	QString valueStyle("font:16px \"Segoe UI\";background:transparent;color:rgb(123,141,167);");
	QString labelStyle("font:16px \"Segoe UI\";background:transparent;");
	unlockTime = new PrivateKeyInMemory(this);

	namespace sm_cmd = keychain_app::secmod_commands;
	auto event_num = shared_event::event_num();
	int time;
	switch(event_num) {
		case (sm_cmd::events_te::sign_hex): {
			auto event = shared_event::ptr<sm_cmd::events_te::sign_hex>();
			time = event.get()->unlock_time;
			break;
		}
		case (sm_cmd::events_te::unlock): {
			auto event = shared_event::ptr<sm_cmd::events_te::unlock>();
			time = event.get()->unlock_time;
			break;
		}
	}

	auto event = shared_event::ptr<sm_cmd::events_te::sign_hash>();

	unlockTime->SetTime(QString::number(time));
}

void UnlockKeyWidget::SetPosition(int x, int y, int width)
{
	unlockTime->SetPosition(0, 0, 116, width);
	currentHeight += 28;
	currentWidth = unlockTime->width();
	setFixedWidth(currentWidth);
	setFixedHeight(currentHeight);
}

int UnlockKeyWidget::GetCurrentHeight() {
	return currentHeight;
}

int UnlockKeyWidget::GetCurrentWidth() {
	return currentWidth;
}

UnlockKeyWidget::~UnlockKeyWidget()
{
}