#ifndef POLLING_H
#define POLLING_H

#include <QObject>
#include <QString>

class Polling : public QObject
{
    Q_OBJECT
public slots:
    void Select();
signals:
    void rx(const std::string);
    void poll();
    void err();
};

#endif // POLLING_H
