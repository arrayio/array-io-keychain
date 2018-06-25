#ifndef POLLING_H
#define POLLING_H

#include <QObject>
#include "cmd.hpp"

class Polling : public QObject
{
    Q_OBJECT
public slots:
    void Select();
    void parse(const std::string&);
    static const gui::cmd_list_singletone cmd_map;
signals:
    void rx(const std::string);
    void poll();
    void err();
};


#endif // POLLING_H
