
#include "SecurityManager.h"
#include <ServiceLogger.h>

using namespace std;

static INT_PTR CALLBACK PasswordProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

SecurityManager::SecurityManager() {

}

SecurityManager::~SecurityManager() {

}

void SecurityManager::CreateSecureDesktop(const std::string& transId) {
	ServiceLogger::getLogger().Log("Start secure desktop");
	HINSTANCE hInst = GetModuleHandle(NULL);
	TCHAR curDirectory[500];
	GetCurrentDirectory(500, curDirectory);
	LPCWSTR commandLine = GetCommandLine();
	wchar_t* found = wcsstr((wchar_t*)commandLine, L"keychain_service_win.exe");
	wchar_t dst[400] = { 0 };
	wchar_t args[400] = { 0 };
	wcsncpy_s(dst, 400, commandLine, (found - (wchar_t*)commandLine));
	wcscat_s(dst, 400, L"keychain_pass_entry_app.exe");
	wcscat_s(args, 400, L"-transId ");
	std::wstring _tId(400, L'#');
	size_t outSize;
	ServiceLogger::getLogger().Log("Create path for start application");
	mbstowcs_s(&outSize, &_tId[0], 400, transId.c_str(), 400);
	wcscat_s(args, 400, _tId.c_str());
	LPCWSTR appToStart = dst;
	LPTSTR app_args = args;
	ServiceLogger::getLogger().Log("CreateSecureDesktop function StartInteractiveClientProcess to enter credentials");
	process = new QProcess();
	QString qtAppStart = QString::fromWCharArray(dst);
	QString qtAppArgs = QString::fromWCharArray(args);
	QStringList argList = { qtAppArgs };
	process->startDetached(qtAppStart, argList);
	//StartInteractiveClientProcess(appToStart, (LPTSTR)app_args);
}

