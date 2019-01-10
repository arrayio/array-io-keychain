#include "PasswordLineEdit.h"

PasswordLineEdit::PasswordLineEdit(e_line_edit win, QWidget * parent)
	: window(win), QLineEdit(parent)
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


void PasswordLineEdit::focusInEvent(QFocusEvent *event)
{
	emit focus(window);
}