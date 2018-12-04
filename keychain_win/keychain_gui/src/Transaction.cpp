#include "Transaction.h"

Transaction::Transaction()
{

}

bool Transaction::json() const
{
	return mJson;
}

void Transaction::setJson(const bool &json) 
{
	mJson = json;
}

QString Transaction::blockchain() const
{
	return mBlockchain;
}

void Transaction::setBlockchain(const QString &blockchain)
{
	mBlockchain = blockchain;
}

QString Transaction::expertMode() const
{
	return mExpertTransaction;
}

void Transaction::setExpertMode(const QString &expertLine)
{
	mExpertTransaction = expertLine;
}

QVector<TransactionParameter> Transaction::transactionParameters() const
{
	return mTransactionParameters;
}

void Transaction::setTransactionParameters(const QVector<TransactionParameter> parameters)
{
	mTransactionParameters = parameters;
}

void Transaction::read(const QJsonObject &jsonObject)
{
	if (jsonObject.contains("blockchain") && jsonObject["blockchain"].isString())
		mBlockchain = jsonObject["blockchain"].toString();
	if (jsonObject.contains("json") && jsonObject["json"].isBool())
		mJson = jsonObject["json"].toBool();
	if (jsonObject.contains("data") && jsonObject["data"].isObject()) {
		QJsonObject dataObject = jsonObject["data"].toObject();
		for (int i = 0; i < dataObject.keys().length(); i++) {
			TransactionParameter tp(dataObject.keys().at(i), dataObject.value(dataObject.keys().at(i)).toString());
			mTransactionParameters.push_back(tp);
		}
	}
}

void Transaction::write(QJsonObject &jsonObject) const
{
	jsonObject["json"] = mJson;
	jsonObject["blockchain"] = mBlockchain;
	QJsonObject dataObject;
	for (int i = 0; i < mTransactionParameters.length(); i++) {
		dataObject.insert(mTransactionParameters.at(i).name(), mTransactionParameters.at(i).value());
	}
	jsonObject["data"] = dataObject;
}

QString Transaction::getValue(QString pName) const
{
	for (int i = 0; i < mTransactionParameters.length(); i++) {
		if (mTransactionParameters.at(i).name() == pName) {
			return mTransactionParameters.at(i).value();
		}
	}
	return QString("");
}