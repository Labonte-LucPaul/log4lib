
install(TARGETS ${PROJECT_NAME}_lib DESTINATION lib)

set(CPACK_GENERATOR "DEB")

# Mandatory package metadata
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VENDOR "free")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_CONTACT "maintainer@example.com")
set(CPACK_PACKAGE_DESCRIPTION "A brief description of my application.")

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON) # Automatically find system dependencies
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)

include(CPack)
