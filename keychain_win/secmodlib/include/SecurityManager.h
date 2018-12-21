#ifndef _SECURITY_MANG_H
#define _SECURITY_MANG_H
#include "Agent.h"
#include <Windows.h>
#include <string>

using namespace std;

class SecurityManager {
public:
	SecurityManager();
	~SecurityManager();
	void CreateSecureDesktop(const int unlock_time=0);
private :
	
	const std::wstring _passEntryAppName = L"keychain_win_gui.exe";
};
#endif