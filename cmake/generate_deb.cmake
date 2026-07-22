include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT ${PROJECT_NAME}Targets
        FILE ${PROJECT_NAME}.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
install(DIRECTORY ${CMAKE_BINARY_DIR}/docs/man/man3
        DESTINATION share/man/man3
        COMPONENT documentation
)

set(CPACK_GENERATOR "DEB")

# Mandatory package metadata
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VENDOR "Luc")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_CONTACT "labonte.lucpaul@gmail.com")
set(CPACK_PACKAGE_DESCRIPTION "Provides logging APIs agnostic to a logging engine.")
set(CPACK_DEBIAN_PACKAGE_SECTION "libdevel")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON) # Automatically find system dependencies
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

set(CPACK_SOURCE_IGNORE_FILES
        libtest/
        tests/
)
include(CPack)
