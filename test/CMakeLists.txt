cmake_minimum_required(VERSION 3.31)
project(psimpl-test  CXX)

# Test suite for psimpl
set(exename "psimpl-test")

# Define the test executable
add_executable(${exename})

target_link_libraries(${exename} PUBLIC
    psimpl::psimpl
)

# Source files
target_sources(${exename} PRIVATE
    # Main and test framework
    main.cpp
    test.cpp

    # Test implementations
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

    # Headers
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

# Installation
install(TARGETS ${exename}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Optional: Enable CTest integration
enable_testing()
add_test(NAME psimpl_tests COMMAND ${exename})
