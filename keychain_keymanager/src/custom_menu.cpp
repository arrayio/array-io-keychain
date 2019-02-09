#include "custom_menu.hpp"

custom_menu::custom_menu(QPushButton* button, QWidget* parent) : QMenu(parent), primaryButton(button)
{
    QMetaObject::connectSlotsByName(this);
    setFixedStyle();
}

void custom_menu::showEvent(QShowEvent* event)
{
    QPoint currentPos = pos();
    QRect geo = primaryButton->geometry();
    move(currentPos.x(), currentPos.y());
}

void const custom_menu::setFixedStyle()
{
    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet("QMenu {background-color:#ffffff;\
                          background:#ffffff;\
                          font-family:\"Segoe UI Normal\";\
                          border-radius:2.5px;\
                          font-size:14px;\
                          border:2px solid #ffffff;}");
    setStyleSheet(styleSheet() + "QMenu::item:selected {background-color:#e7e9ef;\
                                  border: 0px solid #e7e9ef;\
                                  border-radius:1.4px;}" + "QMenu::item {padding:4px;}");
}
