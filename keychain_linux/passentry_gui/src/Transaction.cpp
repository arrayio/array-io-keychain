#include "Transaction.h"

Transaction::Transaction(): unlock_time(-1), key_name(""), is_parsed(false),
blockchain(keychain_app::secmod_commands::blockchain_secmod_te::unknown), rawtx(""),
{};



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