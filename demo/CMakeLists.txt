cmake_minimum_required(VERSION 2.8)
project(psimpl-demo  CXX)

set(exename "psimpl-demo")

###############
# ASI default settings
list(APPEND CMAKE_MODULE_PATH "$ENV{ASV_CMAKE}")
include(asv_cmake_defaults)
asv_cmake_defaults( )
###############

# use our QT
set(QT_LIBINFIX "ASV")

include(GNUInstallDirs)

# use our own library - installed 
find_package(psimpl CONFIG REQUIRED)

find_package(Qt5 CONFIG REQUIRED COMPONENTS
    Gui
    Widgets
)

asv_print_package_properties(
    # psimpl::psimpl
    Qt5::Gui
    Qt5::Widgets
    )

# generate headers to binary dir from ui files
qt5_wrap_ui(
    FORMS_HEADERS
    MainWindow.ui
    )
qt5_add_resources(RESOURCES_RCC icons.qrc)

set(CMAKE_AUTOMOC ON)

add_executable(${exename})
target_include_directories(${exename} PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}   # generated files
    ${CMAKE_CURRENT_SOURCE_DIR})  # RenderArea.h
target_link_libraries(${exename} PUBLIC
    psimpl::psimpl
    Qt5::Gui
    Qt5::Widgets
    )
target_sources(${exename} PRIVATE
    DPWorker.cpp
    main.cpp
    MainWindow.cpp
    RenderArea.cpp

    DPWorker.h
    MainWindow.h
    psimpl_reference.h
    RenderArea.h

    resource.rc

    ${FORMS_HEADERS}
    ${RESOURCES_RCC}
)

install(TARGETS ${exename}
    RUNTIME DESTINATION bin)
