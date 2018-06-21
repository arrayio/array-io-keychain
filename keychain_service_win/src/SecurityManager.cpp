
#include "SecurityManager.h"
#include <ServiceLogger.h>

using namespace std;

static INT_PTR CALLBACK PasswordProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

SecurityManager::SecurityManager() {

}

SecurityManager::~SecurityManager() {

}

void SecurityManager::CreateSecureDesktop(std::wstring transId) {
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
	wcscat_s(args, 400, transId.c_str());
	LPCWSTR appToStart = dst;
	LPTSTR app_args = args;
	ServiceLogger::getLogger().Log("CreateSecureDescktop function StartInteractiveClientProcess to enter credentials");
	StartInteractiveClientProcess(appToStart, (LPTSTR)app_args);
}

