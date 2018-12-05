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

bool Transaction::isSwap() const
{
	return mIsSwap;
}

void Transaction::setSwap(const bool & isSwap)
{
	mIsSwap = isSwap;
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

QVector<TransactionParameter> Transaction::swapTransactionParameters() const
{
	return mSwapTransactionParameters;
}

void Transaction::setSwapTransactionParameters(const QVector<TransactionParameter> parameters)
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
	if (jsonObject.contains("swap") && jsonObject["swap"].isObject()) {
		mIsSwap = true;
		QJsonObject swapObject = jsonObject["swap"].toObject();
		for (int i = 0; i < swapObject.keys().length(); i++) {
			TransactionParameter tp(swapObject.keys().at(i), swapObject.value(swapObject.keys().at(i)).toString());
			mSwapTransactionParameters.push_back(tp);
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
	QJsonObject swapObject;
	for (int i = 0; i < mSwapTransactionParameters.length(); i++) {
		swapObject.insert(mSwapTransactionParameters.at(i).name(), mSwapTransactionParameters.at(i).value());
	}
	jsonObject["swap"] = swapObject;
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

QString Transaction::getSwapValue(QString pName) const
{
	for (int i = 0; i < mSwapTransactionParameters.length(); i++) {
		if (mSwapTransactionParameters.at(i).name() == pName) {
			return mSwapTransactionParameters.at(i).value();
		}
	}
	return QString("");
}