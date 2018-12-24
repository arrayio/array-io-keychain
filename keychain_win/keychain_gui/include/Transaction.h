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
	bool isCreatePassword();
	int isUnlockKey();
	QString unlockKeyName();
	void setCreatePassword();
	void setUnlockKey(QString &key_name, int unlock_time);
private:
	QString sourceTransaction;
	int unlock_time =-1;
	QString key_name = Q_NULLPTR;
	bool createPassword = false;
};


#endif // ! TRANSACTION_H

