#include "Transaction.h"

Transaction::Transaction(QString &transaction)
{
	sourceTransaction = transaction;
	unlock_time = -1;
	key_name = Q_NULLPTR;
}

QString Transaction::getTransactionText()
{
	return sourceTransaction;
}


bool Transaction::isCreatePassword()
{
	return createPassword;
}

int Transaction::isUnlockKey()
{
	if (unlock_time > 0)
		return unlock_time;
	return -1;
}

QString Transaction::unlockKeyName()
{
	if (key_name != Q_NULLPTR)
		return key_name;
	return QString("");
}

void Transaction::setCreatePassword()
{
	createPassword = true;
}

void Transaction::setUnlockKey(QString &Key_name, int Unlock_time)
{
	key_name = Key_name;
	unlock_time = Unlock_time;
}