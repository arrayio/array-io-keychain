#include "Transaction.h"

Transaction::Transaction(QString &transaction)
{
	sourceTransaction = transaction;
}

QString Transaction::getTransactionText()
{
	return sourceTransaction;
}