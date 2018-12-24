#include <fc_light/exception/exception.hpp>
#include <keychain_lib/keychain_logger.hpp>

#include "SecurityManager.h"

using namespace std;



static INT_PTR CALLBACK PasswordProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

SecurityManager::SecurityManager() {

}

SecurityManager::~SecurityManager() {

}

void SecurityManager::CreateSecureDesktop(const int unlock_time) {
	HINSTANCE hInst = GetModuleHandle(NULL);
	TCHAR curDirectory[500];
	
	GetCurrentDirectory(500, curDirectory);
	LPCWSTR commandLine = GetCommandLine();
	std::wstring rem(commandLine);
	std::string::size_type fSymb = rem.find('\"');
	while (fSymb != std::string::npos) {
		rem.erase(fSymb, 1);
		fSymb = rem.find('\"');
	}
	size_t lastOfSlash = rem.find_last_of('\\');
	//wchar_t* found = wcsstr((wchar_t*)rem.c_str(), );
	wchar_t dst[400] = { 0 };
	wcsncpy_s(dst, 400, rem.c_str(), lastOfSlash+1);
	wcscat_s(dst, 400, _passEntryAppName.c_str());
	LPTSTR app_args = L"";
	if (unlock_time > 0) {
		wchar_t args[400] = { 0 };
		wcscat_s(args, 400, L" -unlock_t=");
		std::wstring _tId(400, L'#');
		size_t outSize;
		size_t tSize = rem.length() + to_string(unlock_time).length()+11;
		if (tSize>400) {
			_tId.resize(tSize, '#');
		}
		mbstowcs_s(&outSize, &_tId[0], 400, to_string(unlock_time).c_str(), 400);
		wcscat_s(args, 400, _tId.c_str());
		app_args = args;
	}
	LPCWSTR appToStart = dst;
    auto log = logger_singletone::instance();
    BOOST_LOG_SEV(log.lg, info) << "CreateSecureDescktop function StartInteractiveClientProcess to enter credentials";
	BOOST_LOG_SEV(log.lg, info) << appToStart;
	if (!StartInteractiveClientProcess(appToStart, app_args))
	{
		FC_LIGHT_THROW_EXCEPTION(fc_light::password_input_exception,"Could not create child client process for pass_entry_app");
	}
}

