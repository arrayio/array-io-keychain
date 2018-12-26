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
    void closeEvent(QCloseEvent *);
    void parse(const std::string);
    bool passClearOnExit;
    keychain_gui_win& gui;
public slots:
    void found_pass();
    void send(std::string);
signals:
    void poll();
};
#endif // WIDGET_H
