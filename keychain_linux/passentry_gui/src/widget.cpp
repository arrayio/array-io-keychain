#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QCloseEvent>
#include <fc_light/io/json.hpp>
#include "cmd.hpp"
#include "widget.hpp"

#include <fc_light/reflect/reflect.hpp>
#include <fc_light/variant.hpp>

Q_DECLARE_METATYPE(std::string)

int id = qRegisterMetaType<std::string>();
Widget::Widget(keychain_gui_win& gui_, QWidget *parent)
    :QWidget(parent), gui(gui_)
{

    Polling *polling = new Polling;
    polling->moveToThread(&pollingThread);
    connect(&pollingThread, &QThread::finished, polling, &QObject::deleteLater);

    connect(this,    &Widget::poll,  polling, &Polling::Select, Qt::QueuedConnection);
    connect(polling, &Polling::poll, polling, &Polling::Select, Qt::QueuedConnection);
    connect(polling, &Polling::rx, this, &Widget::parse);
    connect(polling, &Polling::err, this, &Widget::close);

    pollingThread.start();
        emit Widget::poll();
}

Widget::~Widget()
{
    pollingThread.quit();
    pollingThread.wait();
}



void Widget::parse(const std::string s)
{
    auto a = fc_light::json::from_string(s);
    try {
        auto cmd = a.as<slave::cmd_common>();
        auto cmd_map = slave::cmd_list_singletone::instance();
        auto p_func = cmd_map[cmd.cmd];
        (*p_func)(gui, cmd.params);
    }
    catch (const std::exception &e) {throw std::runtime_error(e.what());}
    catch (const fc_light::exception &e) {throw std::runtime_error(e.what());}
}

