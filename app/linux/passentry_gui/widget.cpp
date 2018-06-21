#include "widget.h"
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <unistd.h>
#include <QCloseEvent>

#define KEY_ENTER		28
#define KEY_ESC			1


Widget::Widget(QWidget *parent)
    :QWidget(parent)
{
    setWindowTitle(tr("Promt for password"));
    Interior();

    Polling *polling = new Polling;
    polling->moveToThread(&pollingThread);
    connect(&pollingThread, &QThread::finished, polling, &QObject::deleteLater);

    connect(this,    &Widget::poll,  polling, &Polling::Select, Qt::QueuedConnection);
    connect(polling, &Polling::poll, polling, &Polling::Select, Qt::QueuedConnection);

    connect(polling, &Polling::rx, this, &Widget::cmd);
    connect(polling, &Polling::err, this, &Widget::close);

    pollingThread.start();
    message = KEY_ESC;
    emit Widget::poll();

}

Widget::~Widget()
{
    pollingThread.quit();
    pollingThread.wait();
}

void Widget::Interior()
{
    QGridLayout * grid = new QGridLayout(this);
    {
        QLabel *plb = new QLabel(tr("input password"), this);
        grid->addWidget(plb, 0, 0);
    }
    {
        ple = new QLineEdit(this);
        ple->setEchoMode(QLineEdit::Password);
        grid->addWidget(ple, 0, 1, 1, 3);
    }
    {
        QLabel *plb = new QLabel(tr("raw transaction"), this);
        grid->addWidget(plb, 1, 0);
    }
    {
        QTextEdit * pte = new QTextEdit(this);
        grid->addWidget(pte, 1, 1, 2, 3);
        pte->setText("");
        pte->setReadOnly(true);
    }
    {
        QPushButton *plb = new QPushButton("Ok", this);
        grid->addWidget(plb, 3, 1);
        connect(plb, SIGNAL(clicked()), this, SLOT(mess_ok()));
        connect(plb, SIGNAL(clicked()), this, SLOT(close()));
    }
    {
        QPushButton * ppb = new QPushButton("Cancel", this);
        grid->addWidget(ppb, 3, 2);
        connect(ppb, SIGNAL(clicked()), this, SLOT(close()));
    }
}

void Widget::send(char a)
{
    if (write(STDIN_FILENO, &a, 1) != 1) close();
}

void Widget::mess_ok()
{
    message = KEY_ENTER;
}

void Widget::cmd(const QString& result)
{
   int i =  result.toInt();
   if (i == -1)   close();
   if (i>=0)
   {
       QString s (i, '*');
       ple->setText(s);
   }
}

void Widget::closeEvent(QCloseEvent *event)
{
    send(message);
    event->accept();
}
