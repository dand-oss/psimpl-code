cmake_minimum_required(VERSION 3.12)
# https://dominikberner.ch/cmake-interface-lib/

project(
    "psimpl"
    VERSION 7.1.0
    DESCRIPTION " a lightweight header-only C++ library that is generic, easy to use, and provides a variety of simplification algorithms, including the popular Douglas-Peucker approximation. Algorithms for computing the error induced by simplification are also provided."
    HOMEPAGE_URL "https://github.com/dand-oss/psimpl-code")

set(libname "psimpl")

add_library(${libname} INTERFACE)
# add alias so the project can be uses with add_subdirectory
add_library(${libname}::${libname} ALIAS ${libname})

include(GNUInstallDirs)

target_include_directories(
  ${libname}
  INTERFACE $<BUILD_INTERFACE:${${libname}_SOURCE_DIR}/lib>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_compile_features(${libname} INTERFACE cxx_std_11)

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

# build the demo
#add_subdirectory(demo)
