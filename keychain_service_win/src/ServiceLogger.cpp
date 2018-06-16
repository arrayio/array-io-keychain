#include <ServiceLogger.h>
#include <iostream>
#include <fstream>
#include <Windows.h>

void ServiceLogger::Log(std::string message) {
	ofstream myfile;
	LPCWSTR commandLine = GetCommandLine();
	wchar_t* found = wcsstr((wchar_t*)commandLine, L"keychain_service_win.exe");
	wchar_t dst[400] = { 0 };
	char args[400] = { 0 };
	wcsncpy_s(dst, 400, commandLine, (found - (wchar_t*)commandLine));
	wcscat_s(dst, 400, L"keychainservice.log");
	myfile.open(dst, ios::app);
	if (myfile.is_open()) {
		myfile << message << endl;
		myfile.close();
	}
}
