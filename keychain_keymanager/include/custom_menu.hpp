#ifndef CUSTOM_MENU_HPP
#define CUSTOM_MENU_HPP

#include <QWidget>
#include <QObject>
#include <QPushButton>
#include <QMenu>

class custom_menu : public QMenu
{
    Q_OBJECT

public:
    custom_menu(QPushButton* button, QWidget* parent = Q_NULLPTR);

private:
    QPushButton* primaryButton;

protected:
    void showEvent(QShowEvent* event) override;
};

#endif
