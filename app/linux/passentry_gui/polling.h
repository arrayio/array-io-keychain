#ifndef POLLING_H
#define POLLING_H

#include <QObject>

class Polling : public QObject
{
    Q_OBJECT
public slots:
    void Select();
signals:
    void rx(const QString &result);
    void poll();
    void err();
};


#endif // POLLING_H
