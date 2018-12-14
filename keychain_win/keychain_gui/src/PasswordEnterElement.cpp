#include "PasswordEnterElement.h"

PasswordEnterElement::PasswordEnterElement(bool passwordCreate, QWidget * parent)
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
	pCreatePassword = false;
	if (passwordCreate) {
		pCreatePassword = true;
		description = new QLabel(this);
		description->setWordWrap(true);
		description->setStyleSheet("font:10px \"Segoe UI\";background:transparent;color:rgb(147,148,151);");
		description->setAlignment(Qt::AlignTop | Qt::AlignLeft);
		description->setText("*We recommend to use more than 13 characters, to combine lowercase and uppercase letters, degits and symbols for extra safety (a-z, A-Z, 0-9, @#$%*)");
		
		labelConfirm = new QLabel(this);
		labelConfirm->setFrameStyle(QFrame::NoFrame);
		labelConfirm->setStyleSheet(labelStyle);
		labelConfirm->setAlignment(Qt::AlignBottom | Qt::AlignRight);

		valueConfirm = new QLineEdit(this);
		valueConfirm->setText("");
		valueConfirm->setStyleSheet(passPhraseStyle);
		valueConfirm->setEchoMode(QLineEdit::Password);
		valueConfirm->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

		labelConfirm->setText("Confirm");
		connect(value, &QLineEdit::textEdited, this, &PasswordEnterElement::checkStrength);
		connect(valueConfirm, &QLineEdit::textEdited, this, &PasswordEnterElement::checkConfirm);
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
	if (pCreatePassword) {
		description->move(value->x(), _height);
		description->setFixedSize(value->width(), 35);
		_height+= 40; 
		labelConfirm->move(0, _height);
		labelConfirm->setFixedSize(116, 25);
		valueConfirm->setFixedSize(valueWidth, 25);
		valueConfirm->move(116 + 16, _height);
		_height += 25;
	}
	setFixedSize(116 + valueWidth + 16, _height);
}

void PasswordEnterElement::SetLabel(QString labelValue)
{
	label->setText(labelValue);
}

void PasswordEnterElement::checkStrength(const QString &text)
{
	QRegExp regLetter("[a-zA-Z]+");
	QRegExp regDigit("[0-9]+");
	QRegExp regSymbol("[@#$%*]+");
		if (regSymbol.indexIn(text) != -1) {
			if (text.length() >= 8)
			{
				description->setStyleSheet("font:10px \"Segoe UI\";background:transparent;color:rgb(0,113,0);");
				value->setStyleSheet("font:16px \"Segoe UI\";background-color:rgb(120,255,113);border-style:solid;border-width:1px;border-radius:4px;border-color:rgb(0,113,0);");
				return;
			}
			description->setStyleSheet("font:10px \"Segoe UI\";background:transparent;color:rgb(255,142,4);");
			value->setStyleSheet("font:16px \"Segoe UI\";background-color:rgb(255,230,85);border-style:solid;border-width:1px;border-radius:4px;border-color:rgb(255,142,4);");
			return;
		}
	description->setStyleSheet("font:10px \"Segoe UI\";background:transparent;color:rgb(149,0,101);");
	value->setStyleSheet("font:16px \"Segoe UI\";background-color:rgb(255,140,140);border-style:solid;border-width:1px;border-radius:4px;border-color:rgb(149,0,0);");
	if (pCreatePassword) {
		if (!valueConfirm->text().isEmpty()) {
			if (valueConfirm->text() == text)
				valueConfirm->setStyleSheet("font:16px \"Segoe UI\";background-color:rgb(120,255,113);border-style:solid;border-width:1px;border-radius:4px;border-color:rgb(0,113,0);");
			else
				valueConfirm->setStyleSheet("font:16px \"Segoe UI\";background-color:rgb(255,140,140);border-style:solid;border-width:1px;border-radius:4px;border-color:rgb(149,0,0);");
		}
	}
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

void PasswordEnterElement::checkConfirm(const QString & text)
{
	if (value == Q_NULLPTR)
		return;
	if (value->	text() == text) 
		valueConfirm->setStyleSheet("font:16px \"Segoe UI\";background-color:rgb(120,255,113);border-style:solid;border-width:1px;border-radius:4px;border-color:rgb(0,113,0);");
	else
		valueConfirm->setStyleSheet("font:16px \"Segoe UI\";background-color:rgb(255,140,140);border-style:solid;border-width:1px;border-radius:4px;border-color:rgb(149,0,0);");
}
