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
    void const setFixedStyle();

private:
    QPushButton* primaryButton;
    bool IS_POPUP;

public:
    bool isActive();

public slots:
    void handleShowEvent();

signals:
    void showMenuHandle(const bool IS_POPUP);

protected:
    void showEvent(QShowEvent* event) override;
};

#endif
