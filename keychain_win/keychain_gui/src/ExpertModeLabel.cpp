#include "ExpertModeLabel.h"

ExpertModeLabel::ExpertModeLabel(QWidget * parent)
	:QLabel(parent)
{
	QMetaObject::connectSlotsByName(this);
}

void ExpertModeLabel::mousePressEvent(QMouseEvent * event)
{
	emit clicked();
}