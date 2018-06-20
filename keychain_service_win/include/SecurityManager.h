#ifndef _SECURITY_MANG_H
#define _SECURITY_MANG_H
#include "Agent.h"
#include <Windows.h>

class SecurityManager {
public:
	SecurityManager();
	~SecurityManager();
	void CreateSecureDesktop(std::wstring transId);
private :

};
#endif