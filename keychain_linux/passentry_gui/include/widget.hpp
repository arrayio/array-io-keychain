#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QThread>
#include "polling.hpp"
#include "keychain_gui_win.h"

class Widget : public QWidget
{
    Q_OBJECT
    QThread pollingThread;
public:
    Widget(keychain_gui_win&, QWidget *parent = 0);
    ~Widget();
private:
    void parse(const std::string);
    Polling *polling;

    keychain_gui_win& gui;
signals:
    void poll();
};
#endif // WIDGET_H
