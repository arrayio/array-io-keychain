#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QCloseEvent>
#include <fc/io/json.hpp>
#include "cmd.hpp"
#include "widget.hpp"

#include <fc/reflect/reflect.hpp>
#include <fc/variant.hpp>

Q_DECLARE_METATYPE(std::string)

int id = qRegisterMetaType<std::string>();
Widget::Widget(QWidget *parent)
    :QWidget(parent)
{
    setWindowTitle(tr("Promt for password"));
    interior();

    Polling *polling = new Polling;
    polling->moveToThread(&pollingThread);
    connect(&pollingThread, &QThread::finished, polling, &QObject::deleteLater);

    connect(this,    &Widget::poll,  polling, &Polling::Select, Qt::QueuedConnection);
    connect(polling, &Polling::poll, polling, &Polling::Select, Qt::QueuedConnection);
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

void Widget::interior()
{
    QGridLayout * grid = new QGridLayout(this);
    {
        QLabel *plb = new QLabel(tr("password"), this);
        grid->addWidget(plb, 0, 0);
    }

    ple = new QLineEdit(this);
    ple->setEchoMode(QLineEdit::Password);

    grid->addWidget(ple, 0, 1, 1, 3);
    {
        QLabel *plb = new QLabel(tr("Raw Tx"), this);
        grid->addWidget(plb, 1, 0);
    }

    pte = new QTextEdit(this);
    grid->addWidget(pte, 1, 1, 1, 3);
    pte->setText("");
    pte->setReadOnly(true);

    caps = new QLabel(tr(""), this);
    grid->addWidget(caps, 2, 0);

    num = new QLabel(tr(""), this);
    grid->addWidget(num, 2, 1);

    shift = new QLabel(tr(""), this);
    grid->addWidget(shift, 2, 2);

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
    if ( write(STDIN_FILENO, a.c_str(), sizeof(a.c_str()) ) != a.length() ) close();
}

void Widget::found_pass()
{
  passClearOnExit = false;
}

void Widget::closeEvent(QCloseEvent *event)
{
    passClearOnExit ?
    send(fc::json::to_pretty_string(fc::variant( master::cmd<( master::cmds::cancel)>().base))) :
    send(fc::json::to_pretty_string(fc::variant( master::cmd<( master::cmds::ok)>().base)));
    event->accept();
}

void Widget::parse(const std::string s)
{
    auto a = fc::json::from_string(s);
    try {
        auto cmd = a.as<slave::cmd_common>();
        auto cmd_map = slave::cmd_list_singletone::instance();
        auto p_func = cmd_map[cmd.cmd];
        (*p_func)(this, cmd.params);
    }
    catch (const std::exception &e) {throw std::runtime_error(e.what());}
    catch (const fc::exception &e) {throw std::runtime_error(e.what());}
}

