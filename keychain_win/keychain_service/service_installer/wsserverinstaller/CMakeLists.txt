project( wsservice_installer  )

if(WIN32)

	add_custom_target(wssdeploy)
	find_path(WIX_PATH candle.exe)

	if(NOT WIX_PATH)
		message(FATAL_ERROR "Unable to find WiX in the PATH. The WiX installer will be disabled.")
	else(WIX_PATH)

	set(outputdir "${CMAKE_CURRENT_BINARY_DIR}/wsservice")
	file(MAKE_DIRECTORY "${outputdir}") 
	set(wixsrcpath "${CMAKE_CURRENT_SOURCE_DIR}")
	set(wixoutputpath "wsservice") 
	message(STATUS "wixsrcpath: ${wixsrcpath}")
	
	#find libcrypto.dll file inside openssl installed folder
	if("$ENV{OPENSSL_ROOT_DIR}" OR "${OPENSSL_ROOT_DIR}" STREQUAL "")
		message(FATAL_ERROR "CANNOT FIND OPENSSL LIBRARY")
	else()
		set(openssl_path "${OPENSSL_ROOT_DIR}")
		message(STATUS "OPENSSL PATH: ${openssl_path}")
		file(GLOB_RECURSE globalcrypto FOLLOW_SYMLINKS ${OPENSSL_ROOT_DIR}/libcrypto-1_1-x64.dll)
		list(GET globalcrypto 0 cryptolib)
		if(globalcrypto)
			message(STATUS "FOUND LIBCRYPTO: ${cryptolib}")
		else()
			message(FATAL_ERROR "CANNOT FIND OPENSSL DYNAMIC LIBRARY")
		endif()
	endif()
	
	#find Qt 5 dll files to link and build finally with msi
	message(STATUS "QT ROOT DIR: ${Qt5_DIR}")
	if("$ENV{Qt5_DIR}" OR "${Qt5_DIR}" STREQUAL "")
		message(FATAL_ERROR "CANNOT FIND QT5 LIBRARY")
	else()
		file(GLOB_RECURSE qtlinklibs FOLLOW_SYMLINKS ${Qt5_DIR}/Qt5Widgets.dll 
													 ${Qt5_DIR}/Qt5Core.dll
													 ${Qt5_DIR}/Qt5Gui.dll
													 ${Qt5_DIR}/Qt5Qml.dll
													 ${Qt5_DIR}/Qt5Xml.dll
													 ${Qt5_DIR}/Qt5Svg.dll)
		message(STATUS "QT LIB FILES: ${qtlinklibs}")
		file(GLOB_RECURSE qtlinkplatforms FOLLOW_SYMLINKS ${Qt5_DIR}/plugins/platforms/*.dll)
		message(STATUS "QT PLATFORM FILES: ${qtlinkplatforms}")
	endif()
	
	#set copy files to alias
	set( PREBUILD_FILES
	   "${wixsrcpath}/keychainrunner.bat"
	   "${wixsrcpath}/check_service.bat"
	   "${wixsrcpath}/websocketwrapper.exe"
	   "${wixsrcpath}/servicewrapper.exe"
	   "${wixsrcpath}/servicewrapper.xml"
	   "${wixsrcpath}/wix_wssapp_entry.wxs"
	   "${wixsrcpath}/CustomWixUI_Mondo.wxs"
	   "${wixsrcpath}/CustomInstallDirDlg.wxs"
	   "${wixsrcpath}/CustomLicenseAgreementDlg.wxs"
	   "${wixsrcpath}/en-us.wxl"
	   "${wixsrcpath}/wsservice_manager.bat"
	   "${wixsrcpath}/delete_wsservice.bat"
	   "${wixsrcpath}/wsservicerunner.bat"
	   "${wixsrcpath}/license.rtf"
	   "${wixsrcpath}/keychain_banner.jpg"
	   "${wixsrcpath}/keychain_logo_maximal.ico"
	   "${wixsrcpath}/keychain_logo_minimal.ico"
	   "${wixsrcpath}/keychain_dialog_banner.jpg"
	   ${cryptolib}
	   ${qtlinklibs}
	)

	message(STATUS "PREBUILD FILES: ${PREBUILD_FILES}")
	
	#set alias to demo files
	set( DEMO_FILES
		${CMAKE_CURRENT_SOURCE_DIR}/demo/index.html
		${CMAKE_CURRENT_SOURCE_DIR}/demo/logo.png
		${CMAKE_CURRENT_SOURCE_DIR}/demo/spectre.min.css)
	
	#copy demo files to installer folder
	file(COPY ${DEMO_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/wsservice/demo/)
	#copy qt platform files to installer folder
	file(COPY ${qtlinkplatforms} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/wsservice/platforms/)
	
	#copy files in build folder
	foreach( file_to_copy ${PREBUILD_FILES})
		add_custom_command(TARGET wssdeploy PRE_BUILD COMMAND ${CMAKE_COMMAND}
		ARGS -E copy_if_different ${file_to_copy} "${wixoutputpath}")
	endforeach( file_to_copy )

	set(installerpath "${CMAKE_CURRENT_BINARY_DIR}/wsservice")

	message(STATUS "WSSDeploy Path: ${CMAKE_CURRENT_BINARY_DIR}")
	message(STATUS "WSSDeploy installer Path: ${wixoutputpath}")
	message(STATUS "WIX Path: ${WIX_PATH}")

	# check if you already have a correctly defined PROCESSOR_ARCHITECTURE enviroment variable_requires
	#like: 
	#Intel   - 32-bit x86 
	#Intel64 - Itanium family 
	#AMD64   - 64-bit x86, deprecated 
	#x64     - 64-bit x86 
	
	#get specific platform version to detect vcredist type
	set(TARGET_PLATFORM $ENV{PROCESSOR_ARCHITECTURE})
	message(STATUS "TARGET PLATFORM: ${TARGET_PLATFORM}")
	
	#-bf - flag causes all of the files to be bound int the resulting .wixout/.* file. 
	#-out - flag tells the linker where to output the .wixout/.* file
	#-xo - this flag tells the linker to output an XML representation of the MSI, instead of the actual MSI,  required to use the -bf switch
	add_custom_command(TARGET wssdeploy
					   POST_BUILD
		               COMMAND ${WIX_PATH}/candle.exe 
					   ${installerpath}/wix_wssapp_entry.wxs 
					   ${installerpath}/CustomLicenseAgreementDlg.wxs 
					   ${installerpath}/CustomInstallDirDlg.wxs 
					   ${installerpath}/CustomWixUI_Mondo.wxs 
					   -dProcessorArchitecture=${TARGET_PLATFORM} 
					   -bf -xo -out ${installerpath}/
		               COMMAND ${WIX_PATH}/light.exe 
					   -sice:ICE20
					   -ext WixUIExtension 
					   -ext WixNetfxExtension 
					   ${installerpath}/wix_wssapp_entry.wixobj 
					   ${installerpath}/CustomLicenseAgreementDlg.wixobj 
					   ${installerpath}/CustomInstallDirDlg.wixobj 
					   ${installerpath}/CustomWixUI_Mondo.wixobj 
					   -cultures:en-us -loc ${installerpath}/en-us.wxl 
					   -out ${installerpath}/${PROJECT_NAME}.msi
					   VERBATIM)

	endif()
endif()