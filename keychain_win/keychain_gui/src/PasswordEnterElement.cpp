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

	value = new PasswordLineEdit(this);
	value->setText("");
	value->setStyleSheet(passPhraseStyle);
	value->setEchoMode(QLineEdit::Password);
	value->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	value->setFocus();
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

		valueConfirm = new PasswordLineEdit(this);
		valueConfirm->setText("");
		valueConfirm->setStyleSheet(passPhraseStyle);
		valueConfirm->setEchoMode(QLineEdit::Password);
		valueConfirm->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

		confirmDescription = new QLabel(this);
		confirmDescription->setWordWrap(true);
		confirmDescription->setStyleSheet("font:10px \"Segoe UI\";background:transparent;color:rgb(147,148,151);");
		confirmDescription->setAlignment(Qt::AlignTop | Qt::AlignLeft);
		confirmDescription->setText("");

		labelConfirm->setText("Confirm");
		connect(value, &QLineEdit::textEdited, this, &PasswordEnterElement::checkStrength);
		connect(value, &PasswordLineEdit::finishEnter, this, &PasswordEnterElement::setFocusOnConfirm);
		connect(valueConfirm, &QLineEdit::textEdited, this, &PasswordEnterElement::checkConfirm);
		connect(valueConfirm, &PasswordLineEdit::finishEnter, this, &PasswordEnterElement::checkFinishedEnterance);
	}
	else {
		connect(value, &PasswordLineEdit::finishEnter, this, &PasswordEnterElement::checkFinishedEnterance);
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
		confirmDescription->move(valueConfirm->x(), _height);
		confirmDescription->setFixedSize(valueConfirm->width(), 13);
		_height += 18;
	}
	setFixedSize(116 + valueWidth + 16, _height);
}

void PasswordEnterElement::SetLabel(QString labelValue)
{
	label->setText(labelValue);
}

void PasswordEnterElement::checkStrength(const QString &text)
{
	CheckPasswordStrength passwordChecker;
	switch (passwordChecker.check(text)) {
		case CheckPasswordStrength::strong: {
			description->setStyleSheet("font:10px \"Segoe UI\";background:transparent;color:rgb(0,113,0);");
			value->setStyleSheet("font:16px \"Segoe UI\";background-color:rgb(120,255,113);border-style:solid;border-width:1px;border-radius:4px;border-color:rgb(0,113,0);");
			break;
		}
		case CheckPasswordStrength::middle: {
			description->setStyleSheet("font:10px \"Segoe UI\";background:transparent;color:rgb(255,142,4);");
			value->setStyleSheet("font:16px \"Segoe UI\";background-color:rgb(255,230,85);border-style:solid;border-width:1px;border-radius:4px;border-color:rgb(255,142,4);");
			break;
		}
		case CheckPasswordStrength::weak: {
			description->setStyleSheet("font:10px \"Segoe UI\";background:transparent;color:rgb(149,0,101);");
			value->setStyleSheet("font:16px \"Segoe UI\";background-color:rgb(255,140,140);border-style:solid;border-width:1px;border-radius:4px;border-color:rgb(149,0,0);");
			break;
		}
	}
	if (pCreatePassword) {
		if (!valueConfirm->text().isEmpty()) {
			if (valueConfirm->text() == text) {
				isSame = true;
				valueConfirm->setStyleSheet("font:16px \"Segoe UI\";background-color:rgb(120,255,113);border-style:solid;border-width:1px;border-radius:4px;border-color:rgb(0,113,0);");
			}
			else {
				isSame = false;
				valueConfirm->setStyleSheet("font:16px \"Segoe UI\";background-color:rgb(255,140,140);border-style:solid;border-width:1px;border-radius:4px;border-color:rgb(149,0,0);");
			}
		}
		emit changePassword();
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
	if (value->text() == text) {
		valueConfirm->setStyleSheet("font:16px \"Segoe UI\";background-color:rgb(120,255,113);border-style:solid;border-width:1px;border-radius:4px;border-color:rgb(0,113,0);");
		confirmDescription->setStyleSheet("font:10px \"Segoe UI\";background:transparent;color:rgb(0,113,0);");
		isSame = true;
		confirmDescription->setText("Confirm password ok");
	}
	else {
		confirmDescription->setText("Confirm password error");
		confirmDescription->setStyleSheet("font:10px \"Segoe UI\";background:transparent;color:rgb(149,0,0);");
		isSame = false;
		valueConfirm->setStyleSheet("font:16px \"Segoe UI\";background-color:rgb(255,140,140);border-style:solid;border-width:1px;border-radius:4px;border-color:rgb(149,0,0);");
	}
	emit changePassword();
}


void PasswordEnterElement::setValueFocus()
{
	value->setFocus();
}

void PasswordEnterElement::checkFinishedEnterance()
{
	if (pCreatePassword)
		if (!isSame)
			return;
	emit finishEnterPassword();
}

void PasswordEnterElement::setFocusOnConfirm()
{
	valueConfirm->setFocus();
}

bool PasswordEnterElement::validConfirm()
{
	return isSame;
}
