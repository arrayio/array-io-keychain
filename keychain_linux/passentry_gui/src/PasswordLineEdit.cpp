#include "PasswordLineEdit.h"

PasswordLineEdit::PasswordLineEdit(QWidget * parent)
	: QLineEdit(parent)
{
	QMetaObject::connectSlotsByName(this);
}

PasswordLineEdit::~PasswordLineEdit()
{
}

void PasswordLineEdit::keyPressEvent(QKeyEvent * event)
{
	if (event->key()==Qt::Key_Return) {
		emit finishEnter();
	}
	else
	{
		QLineEdit::keyPressEvent(event);
	}
}
