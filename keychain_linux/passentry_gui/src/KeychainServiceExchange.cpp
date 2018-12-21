#include <KeychainServiceExchange.h>

KeychainServiceExchange::KeychainServiceExchange()
{

}

KeychainServiceExchange::~KeychainServiceExchange()
{

}

void KeychainServiceExchange::EncodeError(const std::wstring &mes, unsigned int len)
{

};

void KeychainServiceExchange::EncodeCancel()
{

};

void KeychainServiceExchange::EncodeSuccess(const std::wstring &mes, unsigned int len)
{

};

bool KeychainServiceExchange::Decode(std::string &outString, int &result)
{
	return true;
};

void KeychainServiceExchange::_sendToPipe(const std::wstring &mes, unsigned int count)
{

};