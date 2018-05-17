To use KeychainService you need to follow this steps.

1. First install KeychainService on PC. For it, start cmd with Administrator's priveligies. In cmd:
	cd <path_to_DummyService.exe>
	<path_to_DummyService.exe>DummyService.exe -install
2. To start service write sc start KeychainService.
3. To check if it's runing sc queryex KeychainService.
4. To check its' work you need to download from folder /bin NamedPipesClient.exe and NamedPipesClient.exe.config.
5. Start NamedPipesClient.exe as Administrator .
6. You will be switched to secure desktop for password enterance.
7. After you will have entered password you will return to you primary desktop.