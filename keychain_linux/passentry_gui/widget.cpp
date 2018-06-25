#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QCloseEvent>
#include <fc/io/json.hpp>
#include "cmd.hpp"
#include "widget.hpp"

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

//    connect(polling, &Polling::rx, this, &Widget::cmd);
    connect(polling, &Polling::rx, this, &Widget::parse);
    connect(polling, &Polling::err, this, &Widget::close);

    pollingThread.start();
    passClearOnExit = true;
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
        pte = new QTextEdit(this);
        grid->addWidget(pte, 1, 1, 2, 3);
        pte->setText("");
        pte->setReadOnly(true);
    }
    {
        QPushButton *plb = new QPushButton("Ok", this);
        grid->addWidget(plb, 3, 1);
        connect(plb, SIGNAL(clicked()), this, SLOT(found_pass()));
        connect(plb, SIGNAL(clicked()), this, SLOT(close()));
    }
    {
        QPushButton * ppb = new QPushButton("Cancel", this);
        grid->addWidget(ppb, 3, 2);
        connect(ppb, SIGNAL(clicked()), this, SLOT(close()));
    }
}

void Widget::send(std::string a)
{
    if ( write(STDIN_FILENO, a.c_str(), sizeof(a.c_str()) ) != 1) close();
}

void Widget::found_pass()
{
  passClearOnExit = false;
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
//    if (passClearOnExit)
//        send(fc::json::to_pretty_string(fc::variant(json_cancel("cansel"))));
//    else
//        send(fc::json::to_pretty_string(fc::variant(json_ok("ok"))));
    event->accept();
}


void Widget::parse(const std::string& s)
{
    auto a = fc::variant(s);
    auto cmd = a.as<gui::cmd_common>();
    auto cmd_map = gui::cmd_list_singletone::instance();
    auto p_func = cmd_map[cmd.cmd];
    (*p_func)(this, cmd.params);
//    std::cout << "pas_len="<< (*p_func)(cmd.params) << std::endl;
}
