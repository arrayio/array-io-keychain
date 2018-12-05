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
	Transaction();

	bool json() const;
	void setJson(const bool &json);

	bool isSwap() const;
	void setSwap(const bool &isSwap);

	QString blockchain() const;
	void setBlockchain(const QString &blockchain);

	QString expertMode() const;
	void setExpertMode(const QString &expertLine);

	QVector<TransactionParameter> transactionParameters() const;
	void setTransactionParameters(const QVector<TransactionParameter> parameters);

	QVector<TransactionParameter> swapTransactionParameters() const;
	void setSwapTransactionParameters(const QVector<TransactionParameter> parameters);

	void read(const QJsonObject &jsonObject);
	void write(QJsonObject &jsonObject) const;

	QString getValue(QString pName) const;
	QString getSwapValue(QString pName) const;

private:
	bool mJson;
	bool mIsSwap=false;
	QString mBlockchain;
	QString mExpertTransaction;
	QVector<TransactionParameter> mTransactionParameters;
	QVector<TransactionParameter> mSwapTransactionParameters;
};


#endif // ! TRANSACTION_H

