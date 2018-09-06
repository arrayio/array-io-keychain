#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>

class QThread;
class Polling;
class Controller: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString mPassword READ password WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(QString mRawTx READ rawTx WRITE setRawTx NOTIFY rawTxChanged)

public:
    explicit Controller(QObject *parent = nullptr);
    ~Controller();

    void setRawTx(const QString &rawTx);
    void setPassword(const QString &password);
    Q_INVOKABLE QString rawTx() const;
    Q_INVOKABLE QString password() const;

signals:
    void poll();
    void close();
    void rawTxChanged();
    void passwordChanged();

public slots:
    void ok();
    void cancel();

private slots:
    void parse(const std::string s);

private:
    Polling *polling;
    QThread *threadPolling;
    QString mPassword;
    QString mRawTx;

    void send(std::string s);

};

#endif // CONTROLLER_H
