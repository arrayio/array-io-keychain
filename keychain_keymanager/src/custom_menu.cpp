#include "custom_menu.hpp"

custom_menu::custom_menu(QPushButton* button, QWidget* parent) : QMenu(parent), primaryButton(button)
{
    QMetaObject::connectSlotsByName(this);
}

void custom_menu::showEvent(QShowEvent* event)
{
    QPoint currentPos = this->pos();
    QRect geo = primaryButton->geometry();
    this->move(currentPos.x(), currentPos.y());
}