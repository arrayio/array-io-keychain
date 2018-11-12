project( service_installer  )

if(WIN32)

	add_custom_target(deploy)
	find_path(WIX_PATH candle.exe)

	if(NOT WIX_PATH)

		message(WARNING "Unable to find WiX in the PATH. The WiX installer will be disabled.")
	  
	else(WIX_PATH)

	set(wixsrcpath "${CMAKE_CURRENT_SOURCE_DIR}")
	set(wixoutputpath "installer")
	message(STATUS "wixsrcpath: ${wixsrcpath}")

	configure_file( ${wixsrcpath}/wix_app_entry.wxs ${wixoutputpath}/wix_app_entry.wxs @ONLY)
	configure_file( ${wixsrcpath}/service_manager.bat ${wixoutputpath}/service_manager.bat @ONLY)
	configure_file( ${wixsrcpath}/delete_service.bat ${wixoutputpath}/delete_service.bat @ONLY)
	configure_file( ${wixsrcpath}/license.rtf ${wixoutputpath}/license.rtf @ONLY)
	configure_file( ${wixsrcpath}/libcrypto-1_1-x64.dll ${wixoutputpath}/libcrypto-1_1-x64.dll @ONLY)

	
	#add_dependencies(deploy keychain_service_win keychain_pass_entry_app)
	#add_custom_command(TARGET keychain_pass_entry_app POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/debug/keychain_pass_entry_app.exe ../service_installer/installer/keychain_pass_entry_app.exe)
	#add_custom_command(TARGET keychain_service_win POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/debug/keychain_service_win.exe ${CMAKE_CURRENT_BINARY_DIR}/service_installer/installer/keychain_service_win.exe)

	set(installerpath "${CMAKE_CURRENT_BINARY_DIR}/installer")

	message(STATUS "Deploy Path: ${CMAKE_CURRENT_BINARY_DIR}")
	message(STATUS "Deploy installer Path: ")
	message(STATUS "WIX Path: ${WIX_PATH}")

	#-bf - flag causes all of the files to be bound int the resulting .wixout/.* file. 
	#-out - flag tells the linker where to output the .wixout/.* file
	#-xo - this flag tells the linker to output an XML representation of the MSI, instead of the actual MSI,  required to use the -bf switch
	
	add_custom_command(TARGET deploy
					   POST_BUILD
		               COMMAND ${WIX_PATH}/candle.exe ${installerpath}/wix_app_entry.wxs -bf -xo -out ${installerpath}/wix_app_entry.wixobj
		               COMMAND ${WIX_PATH}/light.exe -ext WixUIExtension ${installerpath}/wix_app_entry.wixobj -out ${installerpath}/${PROJECT_NAME}.msi
					   VERBATIM)

	endif()
endif()