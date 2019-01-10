project( ws_service_installer  )

if(WIN32)

	add_custom_target(wssdeploy)
	find_path(WIX_PATH candle.exe)

	if(NOT WIX_PATH)
		message(FATAL_ERROR "Unable to find WiX in the PATH. The WiX installer will be disabled.")
	else(WIX_PATH)
	
	add_dependencies(wssdeploy keychain keychain_win_gui)

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
		file(GLOB_RECURSE globalcrypto FOLLOW_SYMLINKS ${OPENSSL_ROOT_DIR}/bin/libcrypto-1_1-x64.dll)
		list(GET globalcrypto 0 cryptolib)
		if(globalcrypto)
			message(STATUS "FOUND LIBCRYPTO: ${cryptolib}")
		else()
			message(FATAL_ERROR "CANNOT FIND OPENSSL DYNAMIC LIBRARY")
		endif()
	endif()

	#find Qt 5 dll files to link and build finally with msi
	set(CMAKE_AUTOMOC ON)
	set(CMAKE_INCLUDE_CURRENT_DIR ON)
	find_package(Qt5 COMPONENTS Core Gui Widgets Qml Xml Svg REQUIRED)
    get_target_property(Qt5_CoreLocation Qt5::Core LOCATION)
	
	message(STATUS "QT5: ${Qt5_CoreLocation}")
	
	if("$ENV{Qt5_CoreLocation}" OR "${Qt5_CoreLocation}" STREQUAL "")
		message(FATAL_ERROR "CANNOT FIND QT5 LIBRARY")
	else()
		add_custom_command(TARGET wssdeploy POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Core>  ${CMAKE_CURRENT_BINARY_DIR}/wsservice
			COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Gui> ${CMAKE_CURRENT_BINARY_DIR}/wsservice
			COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Widgets>  ${CMAKE_CURRENT_BINARY_DIR}/wsservice
			COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Qml>  ${CMAKE_CURRENT_BINARY_DIR}/wsservice
			COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Xml>  ${CMAKE_CURRENT_BINARY_DIR}/wsservice
			COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Svg>  ${CMAKE_CURRENT_BINARY_DIR}/wsservice
			)
		file(GLOB_RECURSE qtlinkplatforms FOLLOW_SYMLINKS ${Qt5_CoreLocation}/../../plugins/platforms/*.dll)
		file(GLOB_RECURSE qtlinkimageformats FOLLOW_SYMLINKS ${Qt5_CoreLocation}/../../plugins/imageformats/*.dll)
		file(GLOB_RECURSE qtlinkstyles FOLLOW_SYMLINKS ${Qt5_CoreLocation}/../../plugins/styles/*.dll)
		message(STATUS "QT PLATFORM FILES: ${qtlinkplatforms}")
		message(STATUS "QT IMAGE FORMATS FILES: ${qtlinkimageformats}")
		message(STATUS "QT STYLES FILES: ${qtlinkstyles}")
	endif()
	
	#set copy files to alias
	set( PREBUILD_FILES
	   "${wixsrcpath}/check_service.bat"
	   "${wixsrcpath}/websocketwrapper.exe"
	   "${wixsrcpath}/servicewrapper.exe"
	   "${wixsrcpath}/servicewrapper.xml"
	   "${wixsrcpath}/wix_wssapp_entry.wxs"
	   "${wixsrcpath}/CustomWixUI_Mondo.wxs"
	   "${wixsrcpath}/CustomInstallDirDlg.wxs"
	   "${wixsrcpath}/CustomVerifyReadyDlg.wxs"
	   "${wixsrcpath}/CustomLicenseAgreementDlg.wxs"
	   "${wixsrcpath}/CustomVersionDlg.wxs"
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
	)

	message(STATUS "PREBUILD FILES: ${PREBUILD_FILES}")
	
	#set alias to demo files
	set( DEMO_FILES
		${CMAKE_CURRENT_SOURCE_DIR}/demo/index.html
		${CMAKE_CURRENT_SOURCE_DIR}/demo/logo.png
		${CMAKE_CURRENT_SOURCE_DIR}/demo/spectre.min.css
		${CMAKE_CURRENT_SOURCE_DIR}/demo/keychain.js)
	
	#copy demo files to installer folder
	file(COPY ${DEMO_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/wsservice/demo/)
	#copy qt platform files to installer folder
	file(COPY ${qtlinkplatforms} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/wsservice/platforms/)
	#copy qt image formats files to installer folder
	file(COPY ${qtlinkimageformats} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/wsservice/imageformats/)
	#copy qt styles files to installer folder
	file(COPY ${qtlinkstyles} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/wsservice/styles/)
	
	#copy files in build folder
	foreach( file_to_copy ${PREBUILD_FILES})
		add_custom_command(TARGET wssdeploy PRE_BUILD COMMAND ${CMAKE_COMMAND}
		ARGS -E copy_if_different ${file_to_copy} "${wixoutputpath}")
	endforeach( file_to_copy )

	set(installerpath "${CMAKE_CURRENT_BINARY_DIR}/wsservice")

	message(STATUS "WSSDeploy Path: ${CMAKE_CURRENT_BINARY_DIR}")
	message(STATUS "WSSDeploy installer Path: ${wixoutputpath}")
	message(STATUS "WIX Path: ${WIX_PATH}")

	#check if you already have a correctly defined PROCESSOR_ARCHITECTURE enviroment variable
	#like: 
	#Intel   - 32-bit x86 
	#Intel64 - Itanium family 
	#AMD64   - 64-bit x86, deprecated 
	#x64     - 64-bit x86 
	
	#get specific platform version to detect vcredist type
	set(TARGET_PLATFORM $ENV{PROCESSOR_ARCHITECTURE})
	message(STATUS "TARGET PLATFORM: ${TARGET_PLATFORM}")
	
	#set installer version
	list( APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/git_version_gen" )
    include( GetGitRevisionDescription )
	git_describe(KEYCHAIN_COMMON_GIT_REVISION_DESCRIPTION --tags)
	SET(VERSION_REGEXP "^[0-9]+\\.[0-9]+(\\-[0-9])*")
	string(REGEX MATCH ${VERSION_REGEXP} VERSION_STRING_GIT ${KEYCHAIN_COMMON_GIT_REVISION_DESCRIPTION})
	string(FIND ${VERSION_STRING_GIT} "-" 3RD_NUM_POS)
	if (${3RD_NUM_POS} EQUAL "-1")
		set(VERSION_STRING ${VERSION_STRING_GIT})
	    string(APPEND VERSION_STRING ".0")
	else()
		string(REPLACE "-" "." VERSION_STRING ${VERSION_STRING_GIT})
	endif()
	set(installerversion ${VERSION_STRING})
	
	#generate new UUID for upgrade code
	set(UUID_NAMESPACE 3238AF6E-1884-4D6B-A32D-14526EF0A389)
	string(
    UUID upgradecode 
    NAMESPACE ${UUID_NAMESPACE} 
    NAME "www.array.io" 
    TYPE SHA1 UPPER
	)

	#use harvester to parse all components 
	#and assign new UUID value for each component
	#INSTALLLOCATION becomes overridable
	#components will be with new UUID values for each build event
	#http://wixtoolset.org/documentation/manual/v3/overview/heat.html
	add_custom_command(TARGET wssdeploy
				   POST_BUILD
				   COMMAND ${WIX_PATH}/heat.exe 
				   dir ${installerpath}/
				   -gg -sfrag -sreg
				   -dr INSTALLLOCATION
				   -out ${installerpath}/basic_components.wxs
				   VERBATIM)
	
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
					   ${installerpath}/CustomVerifyReadyDlg.wxs
					   ${installerpath}/basic_components.wxs
					   ${installerpath}/CustomVersionDlg.wxs
					   -dProcessorArchitecture=${TARGET_PLATFORM}
					   -dUpgradecode=${upgradecode}
					   -dvcrpath=${VCR_PATH}
					   -dproductversion=${installerversion}				   
					   -bf -xo -out ${installerpath}/
		               COMMAND ${WIX_PATH}/light.exe 
					   -sice:ICE20
					   -ext WixUtilExtension
					   -ext WixUIExtension 
					   -ext WixNetfxExtension 
					   ${installerpath}/wix_wssapp_entry.wixobj 
					   ${installerpath}/CustomLicenseAgreementDlg.wixobj 
					   ${installerpath}/CustomInstallDirDlg.wixobj 
					   ${installerpath}/CustomWixUI_Mondo.wixobj 
					   ${installerpath}/CustomVerifyReadyDlg.wixobj
					   ${installerpath}/basic_components.wixobj
					   ${installerpath}/CustomVersionDlg.wixobj
					   -cultures:en-us -loc ${installerpath}/en-us.wxl 
					   -out ${installerpath}/keychain-${installerversion}.msi
					   VERBATIM)
	endif()
endif()
