#include "UnparsedTransactionWidget.h"


UnparsedTransactionWidget::UnparsedTransactionWidget(const std::string& raw_trx, int unlock_time, QWidget * parent)
	:KeychainWidget(parent)
{
	QMetaObject::connectSlotsByName(this);

	QString valueStyle("font:16px \"Segoe UI\";background:transparent;color:rgb(123,141,167);");
	QString labelStyle("font:16px \"Segoe UI\";background:transparent;");

	secmod_parser_f cmd_parse;

	expertModeElement = new ExpertModeElement(this);
	expertModeElement->SetExpertModeText(QString::fromStdString(raw_trx));

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