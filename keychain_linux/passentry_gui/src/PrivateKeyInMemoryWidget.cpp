#include "PrivateKeyInMemoryWidget.h"


PrivateKeyInMemory::PrivateKeyInMemory(QWidget * parent)
	:QWidget(parent)
{
	QMetaObject::connectSlotsByName(this);
	
	timeLabel= new QLabel(this);
	timeLabel->setStyleSheet("font:16px \"Segoe UI\";color:rgb(191,52,52);background:transparent;");
	timeLabel->setText("Unlock time:");
	timeLabel->setFrameStyle(QFrame::NoFrame);
	timeLabel->setAlignment(Qt::AlignBottom | Qt::AlignRight);

	timeValue = new QLabel(this);
	timeValue->setStyleSheet("font:16px \"Segoe UI\";color:rgb(191,52,52);background:transparent;");
	timeValue->setWordWrap(true);
	timeValue->setFrameStyle(QFrame::NoFrame);
	timeValue->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
}

void PrivateKeyInMemory::SetPosition(int x, int y, int labelWidth, int valueWidth)
{
	timeLabel->move(0, 0);
	timeLabel->setFixedWidth(labelWidth);
	timeLabel->setFixedHeight(25);
	timeValue->setFixedWidth(valueWidth);
	timeValue->setFixedHeight(25);
	timeValue->move(labelWidth + 16, 0);
	setFixedWidth(timeLabel->width() + timeValue->width() + 16);
}

void PrivateKeyInMemory::SetTime(QString timeValueStr)
{
	timeValue->setText(timeValueStr + " sec");
}

PrivateKeyInMemory::~PrivateKeyInMemory()
{

}