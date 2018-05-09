#include <windows.h> 
#include <stdlib.h> 
#include <string.h> 
#include <tchar.h> 
#include "..\resource.h"


static INT_PTR CALLBACK PasswordProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	INT_PTR kl = DialogBox(hInstance,                   // application instance
		MAKEINTRESOURCE(IDD_PASSWORD_ENT), // dialog box resource
		NULL,                          // owner window
		PasswordProc                    // dialog box window procedure
	);
}

static INT_PTR CALLBACK PasswordProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	TCHAR lpszPassword[16];
	WORD cchPassword;

	switch (message)
	{
	case WM_INITDIALOG:
		// Set password character to a plus sign (+) 
		SendDlgItemMessage(hDlg,
			IDC_PASS_EDIT,
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
			LOWORD(wParam) == IDC_PASS_EDIT)
		{
			SendMessage(hDlg,
				DM_SETDEFID,
				(WPARAM)IDOK,
				(LPARAM)0);
		}
		switch (wParam)
		{
		case IDOK:
			// Get number of characters. 
			cchPassword = (WORD)SendDlgItemMessage(hDlg,
				IDC_PASS_EDIT,
				EM_LINELENGTH,
				(WPARAM)0,
				(LPARAM)0);
			if (cchPassword >= 16)
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

			// Put the number of characters into first word of buffer. 
			*((LPWORD)lpszPassword) = cchPassword;

			// Get the characters. 
			SendDlgItemMessage(hDlg,
				IDC_PASS_EDIT,
				EM_GETLINE,
				(WPARAM)0,       // line 0 
				(LPARAM)lpszPassword);

			// Null-terminate the string. 
			lpszPassword[cchPassword] = 0;

			MessageBox(hDlg,
				lpszPassword,
				L"Did it work?",
				MB_OK);

			// Call a local password-parsing function. 
			//ParsePassword(lpszPassword);

			EndDialog(hDlg, TRUE);
			return TRUE;

		case IDCANCEL:
			EndDialog(hDlg, TRUE);
			return TRUE;
		}
		return 0;
	}
	return FALSE;

	UNREFERENCED_PARAMETER(lParam);
}