#include "SecureWindowElement.h"

SecureWindowElement::SecureWindowElement(QWidget *parent)
	: QWidget(parent)
{
	//ui.setupUi(this);
	label = new QLabel(this);
	value = new QLabel(this);
	label->setFrameStyle(QFrame::NoFrame);
	value->setFrameStyle(QFrame::NoFrame);
	label->setAlignment(Qt::AlignBottom | Qt::AlignRight);
	value->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
}

SecureWindowElement::~SecureWindowElement()
{
}

void SecureWindowElement::SetPosition(int x, int y, int labelWidth, int valueWidth)
{
	label->move(0, 0);
	label->setFixedWidth(labelWidth);
	value->setFixedWidth(valueWidth);
	label->setFixedHeight(25);
	value->setFixedHeight(25);
	setFixedWidth(label->width()+value->width()+16);
	value->move(labelWidth+16, 0);
	_valueX = labelWidth + 16, 0;
	_labelX = 0;
}

void SecureWindowElement::SetLabelOffset(int offset)
{
	label->move(_labelX+offset, label->y());
	label->setFixedWidth(label->width()-offset);
}

void SecureWindowElement::SetLabelAndValue(QString labelPair) 
{
	QStringList list = labelPair.split(QRegExp("="), QString::SkipEmptyParts);
	if (list[0].indexOf("empty") == -1) {
		label->setText(list[0]);
	}
	value->setText(list[1]);
}

void SecureWindowElement::SetLabelAndValue(QString Label, QString Value)
{
	label->setText(Label);
	value->setText(Value);
}

void SecureWindowElement::SetLabelStyle(QString style)
{
	label->setStyleSheet(style);
}

void SecureWindowElement::SetValueStyle(QString style)
{
	value->setStyleSheet(style);
}