cmake_minimum_required(VERSION 3.31)
project(psimpl-speed  CXX)

# Speed/performance test application for psimpl
set(exename "psimpl-speed")

find_package(Qt6 CONFIG REQUIRED
    COMPONENTS
    Core
)

# Define the executable
add_executable(${exename})

# Link libraries
target_link_libraries(${exename} PUBLIC
    psimpl::psimpl
    Qt::Core
)

# Source files
target_sources(${exename} PRIVATE
    main.cpp
    cycle_p.h
)

# Installation
install(TARGETS ${exename}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
