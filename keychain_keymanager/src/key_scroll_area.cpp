#include "key_scroll_area.hpp"

key_scrooll_area::key_scrooll_area(QWidget *parent)
	:QScrollArea(parent)
{
	QMetaObject::connectSlotsByName(this);
}
