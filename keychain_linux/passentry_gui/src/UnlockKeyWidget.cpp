#include "UnlockKeyWidget.h"

UnlockKeyWidget::UnlockKeyWidget(Transaction &transaction, QWidget * parent)
	:KeychainWidget(parent)
{
	QMetaObject::connectSlotsByName(this);

	QString valueStyle("font:16px \"Segoe UI\";background:transparent;color:rgb(123,141,167);");
	QString labelStyle("font:16px \"Segoe UI\";background:transparent;");
	unlockTime = new PrivateKeyInMemory(this);
	unlockTime->SetTime(QString::number(transaction.isUnlockKey()));
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