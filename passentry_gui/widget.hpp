#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QThread>
#include "polling.hpp"

class Widget : public QWidget
{
    Q_OBJECT
    QThread pollingThread;
public:
    Widget(QWidget *parent = 0);
    ~Widget();
    QLineEdit *ple;
    QTextEdit * pte;
private:
    void Interior();
    void closeEvent(QCloseEvent *);
    void parse(const std::string&);
    bool passClearOnExit;
public slots:
    void cmd(const QString &result);
    void found_pass();
    void send(std::string);
signals:
    void poll();
};
#endif // WIDGET_H
