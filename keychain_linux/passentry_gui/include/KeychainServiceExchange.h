#ifndef KEYCHAINSERVICE_EXCHANGE_H
#define KEYCHAINSERVICE_EXCHANGE_H
#include <string>
#include <stdlib.h>
#include <string.h>

using namespace std;

class KeychainServiceExchange {
public:
	KeychainServiceExchange();
	~KeychainServiceExchange();
	void EncodeError(const std::wstring &mes, unsigned int len);
	void EncodeCancel();
	void EncodeSuccess(const std::wstring &mes, unsigned int len);
	bool Decode(std::string &outString, int &result);
private:
	void _sendToPipe(const std::wstring &mes, unsigned int count);
};
#endif // KEYCHAINSERVICE_EXCHANGE_H