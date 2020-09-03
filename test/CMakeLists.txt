cmake_minimum_required(VERSION 2.8)
project(psimpl-test  CXX)

set(exename "psimpl-test")

include(GNUInstallDirs)

# use our own library - installed 
find_package(psimpl CONFIG REQUIRED)

add_executable(${exename})
target_link_libraries(${exename} PUBLIC
    psimpl::psimpl
    )
target_sources(${exename} PRIVATE
    main.cpp
    test.cpp
    TestDouglasPeucker.cpp
    TestLang.cpp
    TestMath.cpp
    TestNthPoint.cpp
    TestOpheim.cpp
    TestPerpendicularDistance.cpp
    TestPositionalError.cpp
    TestRadialDistance.cpp
    TestReumannWitkam.cpp
    TestUtil.cpp

    helper.h
    TestDouglasPeucker.h
    TestError.h
    test.h
    TestLang.h
    TestMath.h
    TestNthPoint.h
    TestOpheim.h
    TestPerpendicularDistance.h
    TestPositionalError.h
    TestRadialDistance.h
    TestReumannWitkam.h
    TestSimplification.h
    TestUtil.h
)

install(TARGETS ${exename}
    RUNTIME DESTINATION bin)
