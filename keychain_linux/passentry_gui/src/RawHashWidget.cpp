#include "RawHashWidget.h"
#include "widget_singleton.h"


RawHashWidget::RawHashWidget(QWidget * parent)
	:KeychainWidget(parent)
{
	QMetaObject::connectSlotsByName(this);

	QString valueStyle("font:16px \"Segoe UI\";background:transparent;color:rgb(123,141,167);");
	QString labelStyle("font:16px \"Segoe UI\";background:transparent;");

	namespace sm_cmd = keychain_app::secmod_commands;
	auto event = shared_event::ptr<sm_cmd::events_te::sign_hash>();

	from = new SecureWindowElement(this);
	from->SetLabelAndValue("From", QString::fromStdString(event.get()->from));
	from->SetLabelStyle(labelStyle);
	from->SetValueStyle(valueStyle);

	QFont fromFont = from->font();
	fromFont.setPixelSize(16);
	fromFont.setFamily("Segoe UI");
	QString fromStr = QString::fromStdString(event.get()->from);

	QFontMetrics fromFM(fromFont);
	int fromWidth = fromFM.width(fromStr);

	hash = new SecureWindowElement(this);
	hash->SetLabelAndValue("Hash", QString::fromStdString(event.get()->hash));
	hash->SetLabelStyle(labelStyle);
	hash->SetValueStyle(valueStyle);
	QFont hashFont = hash->font();
	QString hashStr = QString::fromStdString(event.get()->hash);
	QFontMetrics hashFM(fromFont);
	int hashWidth = hashFM.width(hashStr);
	if (hashWidth >= fromWidth)
		max_width = hashWidth;
	else
		max_width = fromWidth;
/*
	if (cmd_parse.unlock_time() > 0) {
		unlockTime = new PrivateKeyInMemory(this);
		unlockTime->SetTime(QString::number(cmd_parse.unlock_time()));
	}
*/
	//expertModeElement = new ExpertModeElement(this);
	//expertModeElement->SetExpertModeText(QString::fromStdString(cmd_parse.to_expert_mode_string()), false);

}

void RawHashWidget::SetPosition(int x, int y, int width)
{
	if (max_width > width)
		width = max_width;
	from->SetPosition(0, 0, 116, width+50);
	from->move(0, currentHeight);
	currentHeight += 26;
	hash->SetPosition(0, currentHeight, 116, width);
	hash->move(0, currentHeight);
	currentHeight += 26;
	if (unlockTime != Q_NULLPTR) {
		unlockTime->SetPosition(0, currentHeight, 116, width);
		unlockTime->move(0, currentHeight);
		currentHeight += 36;
	}
	//expertModeElement->SetPosition(0, currentHeight, 116, width);
	//expertModeElement->move(0, currentHeight);
	//currentHeight += 60;
	currentWidth = from->width();
	setFixedWidth(currentWidth);
	setFixedHeight(currentHeight);
}

int RawHashWidget::GetCurrentHeight() {
	return currentHeight;
}

int RawHashWidget::GetCurrentWidth() {
	return currentWidth;
}

RawHashWidget::~RawHashWidget()
{

}