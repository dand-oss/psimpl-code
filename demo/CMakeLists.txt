cmake_minimum_required(VERSION 3.31)
project(psimpl-demo  CXX)

# GUI Demo application for psimpl
set(exename "psimpl-demo")

find_package(Qt6 CONFIG REQUIRED
    COMPONENTS
    Gui
    Widgets
)

# Generate headers to binary dir from ui files
qt6_wrap_ui(FORMS_HEADERS MainWindow.ui)
qt6_add_resources(RESOURCES_RCC icons.qrc)

# Enable automatic MOC processing
set(CMAKE_AUTOMOC ON)

# Define the executable
add_executable(${exename})

# Include directories
target_include_directories(${exename} PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}   # Generated files
    ${CMAKE_CURRENT_SOURCE_DIR}   # RenderArea.h and other headers
)

# Link libraries - use the parent project's library
target_link_libraries(${exename} PUBLIC
    psimpl::psimpl
    Qt::Gui
    Qt::Widgets
)

# Source files
target_sources(${exename} PRIVATE
    # Implementation files
    DPWorker.cpp
    main.cpp
    MainWindow.cpp
    RenderArea.cpp

    # Headers
    DPWorker.h
    MainWindow.h
    psimpl_reference.h
    RenderArea.h

    # Resources
    resource.rc
    ${FORMS_HEADERS}
    ${RESOURCES_RCC}
)

# Installation
install(TARGETS ${exename}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
