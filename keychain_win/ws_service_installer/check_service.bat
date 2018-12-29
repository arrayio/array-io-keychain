@Echo Off
Set ServiceName=Keychain

SC queryex "%ServiceName%"|Find "STATE"|Find /v "RUNNING">Nul&&(
    echo %ServiceName% not running 
    exit /b 0
)||(
    echo "%ServiceName%" working
	echo delete "%ServiceName%"
	delete_wsservice.bat
    exit /b 0
)