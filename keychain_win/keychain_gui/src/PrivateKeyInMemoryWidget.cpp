#include "PrivateKeyInMemoryWidget.h"


PrivateKeyInMemory::PrivateKeyInMemory(const Transaction &transaction, QWidget * parent)
	:KeychainWidget(parent)
{
	QMetaObject::connectSlotsByName(this);
	unlockTime = new SecureWindowElement(this);
	unlockTime->SetLabelStyle("font:16px \"Segoe UI\";background:transparent;");
	unlockTime->SetValueStyle("font:16px \"Segoe UI\";background:transparent;color:rgb(123,141,167);");
	unlockTime->SetLabelAndValue("Unlock time", "300 ms");

}

void PrivateKeyInMemory::SetPosition(int x, int y, int width)
{
	unlockTime->SetPosition(0, 0, 116, width);
	currentHeight = 30;
	setFixedHeight(currentHeight);
}

int PrivateKeyInMemory::GetCurrentHeight() {
	return currentHeight;
}

PrivateKeyInMemory::~PrivateKeyInMemory()
{

}