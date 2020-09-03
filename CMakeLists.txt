cmake_minimum_required(VERSION 3.31)
# https://dominikberner.ch/cmake-interface-lib/

project(
    "psimpl"
    VERSION 7.1.0
    DESCRIPTION "A lightweight header-only C++ library that is generic, easy to use, and provides a variety of simplification algorithms, including the popular Douglas-Peucker approximation. Algorithms for computing the error induced by simplification are also provided."
    HOMEPAGE_URL "https://github.com/dand-oss/psimpl-code")

set(libname "psimpl")

# Options for building demo programs
option(PSIMPL_BUILD_DEMO "Build the psimpl GUI demo application" OFF)
option(PSIMPL_BUILD_TEST "Build the psimpl test suite" ON)
option(PSIMPL_BUILD_SPEED "Build the psimpl speed test application" OFF)
option(PSIMPL_BUILD_ALL_DEMOS "Build all demo programs" OFF)

# If BUILD_ALL_DEMOS is ON, enable all individual demos
if(PSIMPL_BUILD_ALL_DEMOS)
    set(PSIMPL_BUILD_DEMO ON)
    set(PSIMPL_BUILD_TEST ON)
    set(PSIMPL_BUILD_SPEED ON)
endif()

# Header-only library definition
add_library(${libname} INTERFACE)
# Add alias so the project can be used with add_subdirectory
add_library(${libname}::${libname} ALIAS ${libname})

include(GNUInstallDirs)

target_include_directories(
    ${libname}
    INTERFACE $<BUILD_INTERFACE:${${libname}_SOURCE_DIR}/lib>
              $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_compile_features(${libname} INTERFACE cxx_std_11)

# Installation configuration for the library
install(TARGETS ${libname}
        EXPORT ${libname}_Targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${libname}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion)

configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/${libname}Config.cmake.in"
    "${PROJECT_BINARY_DIR}/${libname}Config.cmake"
    INSTALL_DESTINATION
    ${CMAKE_INSTALL_DATAROOTDIR}/${libname}/cmake)

install(EXPORT ${libname}_Targets
    FILE ${libname}Targets.cmake
    NAMESPACE ${libname}::
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${libname}/cmake)

install(FILES "${PROJECT_BINARY_DIR}/${libname}Config.cmake"
    "${PROJECT_BINARY_DIR}/${libname}ConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${libname}/cmake)

install(FILES ${PROJECT_SOURCE_DIR}/lib/psimpl.h DESTINATION include)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/lib/detail DESTINATION include)

set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE.txt")

include(CPack)

# Build demo programs if requested
if(PSIMPL_BUILD_DEMO)
    add_subdirectory(demo)
endif()

if(PSIMPL_BUILD_TEST)
    add_subdirectory(test)
endif()

if(PSIMPL_BUILD_SPEED)
    add_subdirectory(speed)
endif()

# Print summary of build configuration
message(STATUS "")
message(STATUS "psimpl Configuration Summary:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  Build Demo: ${PSIMPL_BUILD_DEMO}")
message(STATUS "  Build Test: ${PSIMPL_BUILD_TEST}")
message(STATUS "  Build Speed: ${PSIMPL_BUILD_SPEED}")
message(STATUS "")
