#include "controller.hpp"
#include "polling.hpp"
#include "cmd.hpp"

#include <fc_light/io/json.hpp>

#include <QThread>
#include <QDebug>

Controller::Controller(QObject *parent):
    QObject(parent)
{
    Polling *polling = new Polling;
    threadPolling = new QThread(this);
    polling->moveToThread(threadPolling);

    connect(threadPolling, &QThread::finished, polling, &QObject::deleteLater);

    connect(this, &Controller::poll, polling, &Polling::Select, Qt::QueuedConnection);
    connect(polling, &Polling::poll, polling, &Polling::Select, Qt::QueuedConnection);
    connect(polling, &Polling::rx, this, &Controller::parse);
    connect(polling, &Polling::err, this, &Controller::close);

    threadPolling->start();
    poll();
}

Controller::~Controller()
{
    threadPolling->quit();
    threadPolling->wait();
}

void Controller::setRawTx(const QString &rawTx)
{
    mRawTx = rawTx;
    emit rawTxChanged();
}

void Controller::setPassword(const QString &password)
{
    mPassword = password;
    emit passwordChanged();
}

QString Controller::rawTx() const
{
    return mRawTx;
}

QString Controller::password() const
{
    return mPassword;
}

void Controller::ok()
{
    qDebug() << "Ok";
    send(fc_light::json::to_pretty_string(fc_light::variant( master::cmd<( master::cmds::ok)>().base)));
}

void Controller::cancel()
{
    qDebug() << "Cancel";
    send(fc_light::json::to_pretty_string(fc_light::variant( master::cmd<( master::cmds::cancel)>().base)));
}

void Controller::parse(const std::string s)
{
    auto a = fc_light::json::from_string(s);
    try {
        auto cmd = a.as<slave::cmd_common>();
        auto cmd_map = slave::cmd_list_singletone::instance();
        auto p_func = cmd_map[cmd.cmd];
        (*p_func)(this, cmd.params);
    }
    catch (const std::exception &e) {throw std::runtime_error(e.what());}
    catch (const fc_light::exception &e) {throw std::runtime_error(e.what());}
}

void Controller::send(std::string a)
{
    if( write(STDIN_FILENO, a.c_str(), a.length() ) != a.length() )
        emit close();
}


