#ifndef  TRANSACTION_PARAMETER_H
#define TRANSACTION_PARAMETER_H

#include <QString>
#include <QJsonObject>

class TransactionParameter
{
public:
	TransactionParameter() = default;
	TransactionParameter(const QString &name, const QString &value);

	QString name() const;
	void setName(const QString &name);

	QString value() const;
	void setValue(const QString &value);

private:
	QString mName;
	QString mValue;
};

#endif // ! TRANSACTION_PARAMETER_H

