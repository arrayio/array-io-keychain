#include "key_scroll_area.hpp"

key_scrooll_area::key_scrooll_area(QWidget *parent)
	:QScrollArea(parent)
{
	QMetaObject::connectSlotsByName(this);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setFrameStyle(QFrame::NoFrame);
	setStyleSheet("border-width:0px;");
}
