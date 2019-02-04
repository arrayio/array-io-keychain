#include <more_button.hpp>

more_button::more_button(QWidget *parent)
	:QPushButton(parent)
{
	QMetaObject::connectSlotsByName(this);
	setCursor(Qt::PointingHandCursor);
	setWindowFlags(Qt::FramelessWindowHint);
	setStyleSheet("border-style:outset;border-width:0px;border-radius:5px;");
	setFixedSize(31,31);
}

void more_button::set_edit_mode()
{
	QIcon editIconPixmap(":/keymanager/edit_icon.png");
	setIcon(editIconPixmap);
	setIconSize(QSize(16,19));
	setStyleSheet("background:transparent;");
}

void more_button::set_remove_mode()
{
	QPixmap trashIconPixmap(":/keymanager/trash_icon.png");
	setIcon(trashIconPixmap);
	setIconSize(QSize(16, 19));
	setStyleSheet("background:transparent;");
}

void more_button::mouseMoveEvent(QMouseEvent * e)
{
	setStyleSheet("background:rgb(231,233,239);");
}

void more_button::leaveEvent(QEvent * event)
{
	setStyleSheet("background:transparent;");
}
