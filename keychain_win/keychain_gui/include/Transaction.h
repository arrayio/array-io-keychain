#ifndef  TRANSACTION_H
#define TRANSACTION_H

#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonObject>
#include <QVector>
#include "TransactionParameter.h"


class Transaction {
public:
	Transaction(QString &transaction);
	QString getTransactionText();
private:
	QString sourceTransaction;
};


#endif // ! TRANSACTION_H

