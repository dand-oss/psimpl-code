TARGET = psimpl-test
TEMPLATE = app
CONFIG += console

HEADERS += \
    TestUtil.h \
    TestSimplification.h \
    TestRadialDistance.h \
    TestNthPoint.h \
    TestMath.h \
    TestError.h \
    test.h \
    helper.h \
    TestPositionalError.h \
    TestPerpendicularDistance.h \
    TestOpheim.h \
    TestLang.h \
    TestDouglasPeucker.h \
    TestReumannWitkam.h \
    ../lib/old_psimpl.h \
    ../lib/psimpl.h \
    ../lib/detail/algo.h \
    ../lib/detail/util.h \
    ../lib/detail/math.h

SOURCES += \
    TestRadialDistance.cpp \
    TestNthPoint.cpp \
    TestMath.cpp \
    test.cpp \
    main.cpp \
    TestUtil.cpp \
    TestReumannWitkam.cpp \
    TestPositionalError.cpp \
    TestPerpendicularDistance.cpp \
    TestOpheim.cpp \
    TestLang.cpp \
    TestDouglasPeucker.cpp
