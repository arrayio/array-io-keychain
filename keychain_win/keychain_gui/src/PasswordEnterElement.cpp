#include "PasswordEnterElement.h"

PasswordEnterElement::PasswordEnterElement(bool withDescription, QWidget * parent)
	:QWidget(parent)
{
	QMetaObject::connectSlotsByName(this); 
	QString labelStyle("font:16px \"Segoe UI\";background:transparent;");
	QString passPhraseStyle("font:16px \"Segoe UI\";background-color:white;border-style:solid;border-width:1px;border-radius:4px;border-color:rgb(225,224,224);");
	label = new QLabel(this);
	label->setFrameStyle(QFrame::NoFrame);
	label->setStyleSheet(labelStyle);
	label->setAlignment(Qt::AlignBottom | Qt::AlignRight);

	value = new QLineEdit(this);
	value->setText("");
	value->setStyleSheet(passPhraseStyle);
	value->setEchoMode(QLineEdit::Password);
	value->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	pWithDescription = false;
	if (withDescription) {
		pWithDescription = true;
		description = new QLabel(this);
		description->setWordWrap(true);
		description->setStyleSheet("font:10px \"Segoe UI\";background:transparent;color:rgb(147,148,151);");
		description->setAlignment(Qt::AlignTop | Qt::AlignLeft);
		description->setText("*We recommend to use more than 13 characters, to combine lowercase and uppercase letters, degits and symbols for extra safety (a-z, A-Z, 0-9, @#$%*)");
	}
}

PasswordEnterElement::~PasswordEnterElement()
{

}

void PasswordEnterElement::SetPosition(int x, int y, int valueWidth)
{
	label->move(0, 0);
	label->setFixedSize(116, 25);
	value->setFixedSize(valueWidth, 25);
	value->move(116 + 16, 0);
	_height = 25;
	if (pWithDescription) {
		description->move(value->x(), _height);
		description->setFixedSize(value->width(), 35);
		_height+= 35;
	}
	setFixedSize(116 + valueWidth + 16, _height);
}

void PasswordEnterElement::SetLabel(QString labelValue)
{
	label->setText(labelValue);
}

bool PasswordEnterElement::IsValid()
{
	QString passwd = value->text();
	QRegExp regSmLetter("[a-z]+");
	QRegExp regBigLetter("[A-Z]+");
	QRegExp regDigit("[0-9]+");
	QRegExp regSymbol("[@#$%*]+");
	if (regSmLetter.indexIn(passwd) != -1 
		&& regBigLetter.indexIn(passwd) != -1
		&& regDigit.indexIn(passwd) != -1 
		&& regSymbol.indexIn(passwd)
		&& passwd.length()>=13) {
		return true;
	}
	description->setStyleSheet("font:10px \"Segoe UI\";background:transparent;color:rgb(240,0,0);");
	return false;
}

void PasswordEnterElement::SetLabelStyle(QString style)
{
	label->setStyleSheet(style);
}

void PasswordEnterElement::SetValueStyle(QString style)
{
	value->setStyleSheet(style);
}

QString PasswordEnterElement::GetValue()
{
	return value->text();
}

int PasswordEnterElement::GetElementHeigth()
{
	return _height;
}
