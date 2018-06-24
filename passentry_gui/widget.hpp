#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QThread>
#include "polling.hpp"

#include <fc/reflect/reflect.hpp>
#include <graphene/chain/protocol/protocol.hpp>
#include <fc/io/json.hpp>

class Widget : public QWidget
{
    Q_OBJECT
    QThread pollingThread;
public:
    Widget(QWidget *parent = 0);
    ~Widget();
    QLineEdit *ple;
    struct json_cancel
    {
        json_cancel(std::string cmd_): cmd(cmd_){};
        std::string cmd;
    };
    struct json_ok
    {
        json_ok(std::string cmd_): cmd(cmd_){};
        std::string cmd;
    };
private:
    void Interior();
    void closeEvent(QCloseEvent *);
    bool passClearOnExit;
public slots:
    void cmd(const QString &result);
    void found_pass();
    void send(std::string);
signals:
    void poll();
};
#endif // WIDGET_H


FC_REFLECT(Widget::json_cancel,  (cmd))
FC_REFLECT(Widget::json_ok,  (cmd))
