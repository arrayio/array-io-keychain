#ifndef MENU_BUTTON_HPP
#define MENU_BUTTON_HPP

#include <QPushButton>
#include <QObject>

class menu_button : public QPushButton
{
	Q_OBJECT
public:
	menu_button(QWidget *parent = Q_NULLPTR);
    void const setFixedStyle();
};

#endif
