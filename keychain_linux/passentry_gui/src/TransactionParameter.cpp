#include "TransactionParameter.h"

TransactionParameter::TransactionParameter(const QString &name, const QString &value)
{
	mName = name;
	mValue = value;
}

QString TransactionParameter::name() const
{
	return mName;
}

void TransactionParameter::setName(const QString &name)
{
	mName = name;
}

QString TransactionParameter::value() const
{
	return mValue;
}

void TransactionParameter::setValue(const QString &value)
{
	mValue = value;
}