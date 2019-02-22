#include "PasswordEnterElement.h"
#include "widget_singleton.h"

PasswordEnterElement::PasswordEnterElement(QWidget * parent)
	:QWidget(parent)
{
	QMetaObject::connectSlotsByName(this); 
	QString labelStyle("font:16px \"Segoe UI\";background:transparent;");
	QString passPhraseStyle("font:16px \"Segoe UI\";background-color:white;border-style:solid;border-width:1px;border-radius:4px;border-color:rgb(225,224,224);");
	label = new QLabel(this);
	label->setFrameStyle(QFrame::NoFrame);
	label->setStyleSheet(labelStyle);
	label->setAlignment(Qt::AlignBottom | Qt::AlignRight);

	value = new PasswordLineEdit(main_line, this);
	value->setText("");
	value->setStyleSheet(passPhraseStyle);
	value->setEchoMode(QLineEdit::Password);
	value->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	value->setFocus();
	namespace sm_cmd = keychain_app::secmod_commands;
	auto event_num = shared_event::event_num();

	if (event_num == sm_cmd::events_te::create_key) {
		description = new QLabel(this);
		description->setWordWrap(true);
		description->setStyleSheet("font:10px \"Segoe UI\";background:transparent;color:rgb(147,148,151);");
		description->setAlignment(Qt::AlignTop | Qt::AlignLeft);
		description->setText("*We recommend to use more than 13 characters, to combine lowercase and uppercase letters, degits and symbols for extra safety (a-z, A-Z, 0-9, @#$%*)");
		
		labelConfirm = new QLabel(this);
		labelConfirm->setFrameStyle(QFrame::NoFrame);
		labelConfirm->setStyleSheet(labelStyle);
		labelConfirm->setAlignment(Qt::AlignBottom | Qt::AlignRight);

		valueConfirm = new PasswordLineEdit(confirm_line, this);
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
		//connect(value, &QLineEdit::textEdited, this, &PasswordEnterElement::checkStrength);
//		connect(value, &PasswordLineEdit::finishEnter, this, &PasswordEnterElement::setFocusOnConfirm);
//		connect(valueConfirm, &QLineEdit::textEdited, this, &PasswordEnterElement::checkConfirm);
//		connect(valueConfirm, &PasswordLineEdit::finishEnter, this, &PasswordEnterElement::checkFinishedEnterance);
		connect(value, &PasswordLineEdit::focus, this, &PasswordEnterElement::tabFocusByMouse);
        connect(valueConfirm, &PasswordLineEdit::focus, this, &PasswordEnterElement::tabFocusByMouse);
	}
	else {
		//connect(value, &PasswordLineEdit::finishEnter, this, &PasswordEnterElement::checkFinishedEnterance);
	}
}


void PasswordEnterElement::tabFocusByMouse(int line)
{
    emit focus(line);
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
    namespace sm_cmd = keychain_app::secmod_commands;
    auto event_num = shared_event::event_num();

    if (event_num == sm_cmd::events_te::create_key) {
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

void PasswordEnterElement::checkStrength(strength_te strength)
{
	switch (strength)
	{
	    case strength_te::weak:
		{
			description->setStyleSheet("font:10px \"Segoe UI\";background:transparent;color:rgb(158,37,17);");
			value->setStyleSheet("font:16px \"Segoe UI\";background-color:rgb(230,201,201);border-style:solid;border-width:1px;border-radius:4px;border-color:rgb(158,37,17);");
			break;
		}
		case strength_te::middle:
		{
			description->setStyleSheet("font:10px \"Segoe UI\";background:transparent;color:rgb(255,142,4);");
			value->setStyleSheet("font:16px \"Segoe UI\";background-color:rgb(240,230,180);border-style:solid;border-width:1px;border-radius:4px;border-color:rgb(255,142,4);");
			break;
		}
		case strength_te::strong:
		{
			description->setStyleSheet("font:10px \"Segoe UI\";background:transparent;color:rgb(82,172,75);");
			value->setStyleSheet("font:16px \"Segoe UI\";background-color:rgb(195,231,192);border-style:solid;border-width:1px;border-radius:4px;border-color:rgb(82,172,75);");
			break;
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

void PasswordEnterElement::checkConfirm(const bool confirm)
{
    namespace sm_cmd = keychain_app::secmod_commands;
    auto event_num = shared_event::event_num();

    if (event_num == sm_cmd::events_te::create_key)
	{
		isSame = confirm;
		if (confirm) {
			valueConfirm->setStyleSheet("font:16px \"Segoe UI\";background-color:rgb(195,231,192);border-style:solid;border-width:1px;border-radius:4px;border-color:rgb(82,172,75);");
			confirmDescription->setStyleSheet("font:10px \"Segoe UI\";background:transparent;color:rgb(82,172,75);");
			confirmDescription->setText("Confirm password ok");
		}
		else {
			confirmDescription->setText("Confirm password error");
			confirmDescription->setStyleSheet("font:10px \"Segoe UI\";background:transparent;color:rgb(158,37,17);");
			valueConfirm->setStyleSheet("font:16px \"Segoe UI\";background-color:rgb(230,201,201);border-style:solid;border-width:1px;border-radius:4px;border-color:rgb(158,37,17);");
		}
	}
}


void PasswordEnterElement::setValueFocus()
{
	value->setFocus();
}

void PasswordEnterElement::checkFinishedEnterance()
{
/*	if (pCreatePassword)
		if (!isSame)
			return;
	emit finishEnterPassword();
 */
}

void PasswordEnterElement::setFocusOnConfirm()
{
	valueConfirm->setFocus();
}

bool PasswordEnterElement::validConfirm()
{
	return isSame;
}
