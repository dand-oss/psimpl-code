cmake_minimum_required(VERSION 2.8)
project(psimpl-speed  CXX)

set(exename "psimpl-speed")

include(GNUInstallDirs)

# use our own library - installed 
find_package(psimpl CONFIG REQUIRED)

find_package(Qt5 CONFIG REQUIRED COMPONENTS
    Core
)

add_executable(${exename})
target_link_libraries(${exename} PUBLIC
    psimpl::psimpl
    Qt5::Core
    )
target_sources(${exename} PRIVATE
    main.cpp
    clocktic.h
)

install(TARGETS ${exename}
    RUNTIME DESTINATION bin)
