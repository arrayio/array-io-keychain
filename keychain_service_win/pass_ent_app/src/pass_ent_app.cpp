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
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "Userenv.lib")
#pragma comment(lib, "Wtsapi32.lib")

#define DESKTOP_ALL (DESKTOP_READOBJECTS | DESKTOP_CREATEWINDOW | \
DESKTOP_CREATEMENU | DESKTOP_HOOKCONTROL | DESKTOP_JOURNALRECORD | \
DESKTOP_JOURNALPLAYBACK | DESKTOP_ENUMERATE | DESKTOP_WRITEOBJECTS | \
DESKTOP_SWITCHDESKTOP | STANDARD_RIGHTS_REQUIRED)

#include <stdio.h>
#include "..\resource.h"

using namespace std;

INT_PTR CALLBACK PasswordProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

std::wstring TRANS_ID = TEXT("-transId");
std::wstring TransId;
HDESK hOldDesktop, hNewDesktop;
DWORD WINAPI DrawWindow(LPVOID);
HINSTANCE _hInstance;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	std::size_t transIdLen = TRANS_ID.length();
	std::size_t totalLen = strlen(lpCmdLine);
	std::wstring wc(200, L'#');
	size_t outSize;
	mbstowcs_s(&outSize, &wc[0], 200, lpCmdLine, 200);
	TransId = wc.substr(transIdLen, totalLen - transIdLen);

	//hOldDesktop = GetThreadDesktop(GetCurrentThreadId());
	//hNewDesktop = CreateDesktop(L"secdesktop", NULL, NULL, 0, DESKTOP_ALL, NULL);

	//SwitchDesktop(hNewDesktop);

	string passwd = "";
	HANDLE thread = CreateThread(NULL, 0, DrawWindow, NULL, 0, NULL);
	
	WaitForSingleObject(thread, INFINITE);
	//SwitchDesktop(hOldDesktop);

	//CloseDesktop(hNewDesktop);
}

static DWORD WINAPI DrawWindow(LPVOID t)
{
	SetThreadDesktop(hNewDesktop);
	INT_PTR kl = DialogBoxW(_hInstance,                   // application instance
		MAKEINTRESOURCE(IDD_PASSENTERANCE), // dialog box resource
		NULL,                          // owner window
		PasswordProc                   // dialog box window procedure
	);
	return 0;
}

static INT_PTR CALLBACK PasswordProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	WORD cchPassword;  //количество введёных символов
	wchar_t read_password[36];

	switch (message)
	{
	case WM_INITDIALOG:
		SendDlgItemMessage(hDlg,
			IDC_TRANSACTIONID,
			WM_SETTEXT,
			NULL,
			(LPARAM)TransId.c_str()
		);
		// Set password character to a plus sign (+) 
		SendDlgItemMessage(hDlg,
			IDC_PASSWORD,
			EM_SETPASSWORDCHAR,
			(WPARAM) '*',
			(LPARAM)0);

		// Set the default push button to "Cancel." 
		SendMessage(hDlg,
			DM_SETDEFID,
			(WPARAM)IDCANCEL,
			(LPARAM)0);

		return TRUE;

	case WM_COMMAND:
		// Set the default push button to "OK" when the user enters text. 
		if (HIWORD(wParam) == EN_CHANGE &&
			LOWORD(wParam) == IDC_PASSWORD)
		{
			SendMessage(hDlg,
				DM_SETDEFID,
				(WPARAM)IDOK,
				(LPARAM)0);
		}
		switch (wParam)
		{
		case IDOK: {
			// Get number of characters. 
			cchPassword = (WORD)SendDlgItemMessage(hDlg,
				IDC_PASSWORD,
				EM_LINELENGTH,
				(WPARAM)0,
				(LPARAM)0);
			if (cchPassword > 16)
			{
				MessageBox(hDlg,
					L"Too many characters.",
					L"Error",
					MB_OK);

				EndDialog(hDlg, TRUE);
				return FALSE;
			}
			else if (cchPassword == 0)
			{
				MessageBox(hDlg,
					L"No characters entered.",
					L"Error",
					MB_OK);

				EndDialog(hDlg, TRUE);
				return FALSE;
			}

			//read_password[0] = cchPassword;
			// Get the characters. 
			SendDlgItemMessage(hDlg,
				IDC_PASSWORD,
				EM_GETLINE,
				(WPARAM)0,       // line 0 
				(LPARAM)&read_password); //чтение пароля из textbox'a 

			// Null-terminate the string. 
			

			HANDLE hPipe;
			DWORD dwWritten;
			hPipe = CreateFile(TEXT("\\\\.\\pipe\\keychainpass"),
				GENERIC_READ | GENERIC_WRITE,
				0,
				NULL,
				OPEN_EXISTING,
				0,
				NULL);
			int len;
			DWORD error = GetLastError();
 			len = WideCharToMultiByte(CP_ACP, 0, read_password, cchPassword, 0, 0, 0, 0);
			std::string testStr(len, '\0');
			WideCharToMultiByte(CP_ACP, 0, read_password, cchPassword, &testStr[0], len, 0, 0);
			DATA_BLOB DataIn;
			DATA_BLOB DataOut;
			

			DataIn.pbData = (BYTE*)testStr.c_str();
			DataIn.cbData = len;

			if (CryptProtectData(
				&DataIn,
				L"This is the description string.", // A description string to be included with the encrypted data. 
				NULL,                               // Optional entropy not used.
				NULL,                               // Reserved.
				NULL,                               // Pass NULL for the prompt structure.
				CRYPTPROTECT_LOCAL_MACHINE,
				&DataOut))
			{
				printf("The encryption phase worked.\n");
			}
			else
			{
				printf("Encryption error using CryptProtectData.\n");
			}
			
			
			if (hPipe != INVALID_HANDLE_VALUE)
			{
				WriteFile(hPipe,
					DataOut.pbData,//testStr.c_str(),
					DataOut.cbData+1,//len+1,   // = length of string + terminating '\0' !!!
					&dwWritten,
					NULL);
			}
			CloseHandle(hPipe);
			testStr.~basic_string();
			//LocalFree(read_password);
			// Call a local password-parsing function. 
			//ParsePassword(lpszPassword);

			EndDialog(hDlg, TRUE);
			return TRUE;
		}
		case IDCANCEL:
			EndDialog(hDlg, TRUE);
			return TRUE;
		}
		return 0;
	}
	return FALSE;

	UNREFERENCED_PARAMETER(lParam);
}