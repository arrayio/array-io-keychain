#ifndef  TRANSACTION_H
#define TRANSACTION_H

#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonObject>
#include <QVector>
#include "TransactionParameter.h"
#include "secmod_protocol.hpp"


class Transaction {
public:
    Transaction();
	QString getTransactionText();
	bool isCreatePassword();
	int isUnlockKey();
	QString unlockKeyName();
	void setCreatePassword();
	void setUnlockKey(QString &key_name, int unlock_time);

private:
	bool createPassword = false;
};


#endif // ! TRANSACTION_H

