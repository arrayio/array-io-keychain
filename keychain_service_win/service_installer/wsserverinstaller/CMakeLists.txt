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

	#TODO
	#add keychain executable copy to wsservice folder ~ global path

	#set copy files to alias
	set( PREBUILD_FILES
	   "${wixsrcpath}/websocketwrapper.exe"
	   "${wixsrcpath}/servicewrapper.exe"
	   "${wixsrcpath}/servicewrapper.xml"
	   "${wixsrcpath}/wix_wssapp_entry.wxs"
	   "${wixsrcpath}/wsservice_manager.bat"
	   "${wixsrcpath}/delete_wsservice.bat"
	   "${wixsrcpath}/wsservicerunner.bat"
	   "${wixsrcpath}/license.rtf"
	   "${wixsrcpath}/keychain_banner.jpg"
	   "${wixsrcpath}/keychain_logo_maximal.ico"
	   "${wixsrcpath}/keychain_logo_minimal.ico"
	   "${wixsrcpath}/keychain_dialog_banner.jpg"
	)

	message(STATUS "PREBUILD FILES: ${PREBUILD_FILES}")

	#copy files in build folder
	foreach( file_to_copy ${PREBUILD_FILES})
		add_custom_command(TARGET wssdeploy PRE_BUILD COMMAND ${CMAKE_COMMAND}
		ARGS -E copy_if_different ${file_to_copy} "${wixoutputpath}")
	endforeach( file_to_copy )

	set(installerpath "${CMAKE_CURRENT_BINARY_DIR}/wsservice")

	message(STATUS "WSSDeploy Path: ${CMAKE_CURRENT_BINARY_DIR}")
	message(STATUS "WSSDeploy installer Path: ${wixoutputpath}")
	message(STATUS "WIX Path: ${WIX_PATH}")

	#-bf - flag causes all of the files to be bound int the resulting .wixout/.* file. 
	#-out - flag tells the linker where to output the .wixout/.* file
	#-xo - this flag tells the linker to output an XML representation of the MSI, instead of the actual MSI,  required to use the -bf switch
	
	add_custom_command(TARGET wssdeploy
					   POST_BUILD
		               COMMAND ${WIX_PATH}/candle.exe ${installerpath}/wix_wssapp_entry.wxs -bf -xo -out ${installerpath}/wix_wssapp_entry.wixobj
		               COMMAND ${WIX_PATH}/light.exe -ext WixUIExtension ${installerpath}/wix_wssapp_entry.wixobj -out ${installerpath}/${PROJECT_NAME}.msi
					   VERBATIM)

	endif()
endif()