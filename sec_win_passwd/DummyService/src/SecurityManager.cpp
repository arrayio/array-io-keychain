#include "SecurityManager.h"
#include "Agent.h"

static INT_PTR CALLBACK PasswordProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

SecurityManager::SecurityManager() {

}

SecurityManager::~SecurityManager() {

}

void SecurityManager::CreateSecureDesktop() {
	HINSTANCE hInst = GetModuleHandle(NULL);
	LPCWSTR commandLine = GetCommandLine();
	wchar_t* found = wcsstr((wchar_t*)commandLine, L"DummyService.exe");
	wchar_t dst[400] = { 0 };
	wcsncpy_s(dst, 400, commandLine, (found - (wchar_t*)commandLine));
	wcscat_s(dst, 400, L"pass_ent_app.exe");
	LPCWSTR appToStart = dst;
	//L"C:\\MyProjects\\arrayiopasswin\\array-io-keychain\\sec_win_passwd\\DummyService\\Debug\\pass_ent_app.exe"
	StartInteractiveClientProcess(appToStart, (LPTSTR)TEXT("-transId jdlksjflfskdljfaldjkfsajf7f8f68as7dfhajsfhk"));
}