DummyWindowsService

First you nedd simply start application bin\DummyService.exe. Then you have to start netpipeclient and initiate connection to \\.\pipe\keychainservice. Application shows you window for entering password.

First you need to build DummyService project.
To install it, please start command line in administrtor mode and type path to DummyKeychainService.exe with argument /install or -install.

After above steps you can check if it is running with command sc queryex DummyKeyChainService.
