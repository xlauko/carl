
if(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} VERSION_GREATER 3.0)

	include(GNUInstallDirs)

	install(
		DIRECTORY ${CMAKE_SOURCE_DIR}/src/carl/
		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/carl
		FILES_MATCHING REGEX ".*\.(h|tpp)$"
	)

	install(
		TARGETS carl-shared carl-static
		EXPORT carl_Targets
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	)

	install(FILES ${CMAKE_BINARY_DIR}/carlConfig.install.cmake DESTINATION ${CMAKE_INSTALL_DIR} RENAME carlConfig.cmake)
	install(FILES ${CMAKE_BINARY_DIR}/carlConfigVersion.cmake DESTINATION ${CMAKE_INSTALL_DIR})
	install(EXPORT carl_Targets FILE carlTargets.cmake DESTINATION ${CMAKE_INSTALL_DIR})
else()
	message(STATUS "Disabled install target due to cmake version less than 3.1")
endif()
