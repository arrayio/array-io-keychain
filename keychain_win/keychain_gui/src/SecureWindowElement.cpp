#include "SecureWindowElement.h"

SecureWindowElement::SecureWindowElement(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
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
	label->move(x, y);
	label->setFixedWidth(labelWidth);
	value->setFixedWidth(valueWidth);
	label->setFixedHeight(25);
	value->setFixedHeight(25);
	setFixedWidth(label->width()+value->width()+16);
	value->move(x+labelWidth+16, y);
	_valueX = x + labelWidth + 16, y;
	_labelX = x;
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