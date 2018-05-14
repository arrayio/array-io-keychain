#include "SecurityManager.h"
#include "..\resource.h"
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
	//STARTUPINFO si;
	//PROCESS_INFORMATION pi;
	//ZeroMemory(&si, sizeof(si));
	//si.cb = sizeof(si);
	//ZeroMemory(&pi, sizeof(pi));
	//LPTSTR sysProc = (LPTSTR)TEXT("C:\\MyProjects\\arrayiopasswin\\array-io-keychain\\sec_win_passwd\\DummyService\\Debug\\sec_enter_pass.exe");
	//if (!CreateProcessW(sysProc,   // No module name (use command line)
	//	NULL,        // Command l
	//	NULL,           // Process handle not inheritable
	//	NULL,           // Thread handle not inheritable
	//	FALSE,          // Set handle inheritance to FALSE
	//	0,              // No creation flags
	//	NULL,           // Use parent's environment block
	//	NULL,           // Use parent's starting directory 
	//	&si,            // Pointer to STARTUPINFO structure
	//	&pi)           // Pointer to PROCESS_INFORMATION structure
	//	);
	/*INT_PTR kl = DialogBox(hInst,                   // application instance
		MAKEINTRESOURCE(IDE_PASSWORD), // dialog box resource
		NULL,                          // owner window
		PasswordProc                    // dialog box window procedure
	);*/
	//WaitForSingleObject(pi.hProcess, INFINITE);

	// Close process and thread handles. 
	//CloseHandle(pi.hProcess);
	//CloseHandle(pi.hThread);
}