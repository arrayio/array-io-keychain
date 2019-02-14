#include "menu_button.hpp"

menu_button::menu_button(QWidget *parent)
	:QPushButton(parent)
{
	QMetaObject::connectSlotsByName(this);
    setFixedStyle();
}

void const menu_button::setFixedStyle()
{
    setStyleSheet("QPushButton:hover{background-color:#e7e9ef;\
                                     border-style:inset; \
                                     border-radius:2px 2px 2px 2px; \
                                     border:2px solid #e7e9ef;}");
    setStyleSheet(styleSheet() + "QPushButton::menu-indicator{image: none;}");
}
