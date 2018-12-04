#ifndef KEYCHAINSERVICE_EXCHANGE_H
#define KEYCHAINSERVICE_EXCHANGE_H
#include <string>
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <cstddef>
#include <clocale>
#include <locale>
#include <codecvt>
#include <vector>
#include <tchar.h>
#include <windows.h>
#include <UserEnv.h>
#include <Tchar.h>
#include <string>
#include <cwchar>
#include <WtsApi32.h>

using namespace std;

class KeychainServiceExchange {
public:
	KeychainServiceExchange();
	~KeychainServiceExchange();
	void EncodeError(const std::wstring &mes, unsigned int len);
	void EncodeSuccess(const std::wstring &mes, unsigned int len);
	bool Decode(std::string &outString, int &result);
private:
	void _sendToPipe(const std::wstring &mes, unsigned int count);
};
#endif // KEYCHAINSERVICE_EXCHANGE_H