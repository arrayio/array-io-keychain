#ifndef _SECURITY_MANG_H
#define _SECURITY_MANG_H
#include "Agent.h"
#include <Windows.h>
#include <QWidget>
#include <QProcess>
#include <QApplication>

class SecurityManager {
public:
	SecurityManager();
	~SecurityManager();
	void CreateSecureDesktop(const std::string& transId);
private :
	QProcess * process;
};
#endif