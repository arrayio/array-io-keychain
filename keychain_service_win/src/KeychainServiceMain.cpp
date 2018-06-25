#include <wchar.h>
#include <thread>
#include <conio.h>
#include "ServiceInstaller.h"
#include "KeychainService.h"
#include "ServiceBase.h"
#include <ServiceLogger.h>

// Internal name of the service 
#define SERVICE_NAME             L"KeyChainService" 

// Displayed name of the service 
#define SERVICE_DISPLAY_NAME     L"KeyChainService" 
#define SERVICE_DESCRIPTION     L"This service shows window for enterance password" 

// Service start options. 
#define SERVICE_START_TYPE       SERVICE_DEMAND_START 

// List of service dependencies - "dep1\0dep2\0\0" 
#define SERVICE_DEPENDENCIES     L"" 

// The name of the account under which the service should run 
#define SERVICE_ACCOUNT          L"NT AUTHORITY\\LocalService" 

// The password to the service account name 
#define SERVICE_PASSWORD         NULL 

int main(int argc, char *argv[])
{
	if ((argc > 1) && ((*argv[1] == '-' || (*argv[1] == '/'))))
	{
		if (_stricmp("install", argv[1] + 1) == 0)
		{
			// Install the service when the command is  
			// "-install" or "/install". 
			InstallService(
				(PWSTR)SERVICE_NAME,               // Name of service 
				(PWSTR)SERVICE_DISPLAY_NAME,       // Name to display 
				SERVICE_START_TYPE,         // Service start type 
				(PWSTR)SERVICE_DEPENDENCIES,       // Dependencies 
				(PWSTR)SERVICE_ACCOUNT,            // Service running account 
				SERVICE_PASSWORD            // Password of the account 
			);
		}
		else if (_stricmp("remove", argv[1] + 1) == 0)
		{
			// Uninstall the service when the command is  
			// "-remove" or "/remove". 
			UninstallService((PWSTR)SERVICE_NAME);
		}
		else if (_stricmp("r", argv[1] + 1) == 0) {
			KeychainService service((PWSTR)SERVICE_NAME);
			ServiceLogger::getLogger().Log("KeychainService started");
			if (!CServiceBase::Run(service))
			{
				ServiceLogger::getLogger().Log("Starting Error");
				ServiceLogger::getLogger().Log(std::to_string(GetLastError()));
				wprintf(L"Service failed to run w/err 0x%08lx\n", GetLastError());
			}
		}
	}
	else
	{
		wprintf(L"Parameters:\n");
		wprintf(L" -install  to install the service.\n");
		wprintf(L" -remove   to remove the service.\n");
	}
	return 0;
}