TARGET = psimpl-speed
TEMPLATE = app
CONFIG += \
    console \
    qtestlib

SOURCES += \
    main.cpp

HEADERS += \
    ../lib/psimpl.h \
    ../demo/psimpl_reference.h \
    cycle_p.h \
    prev/psimpl.h \
    ../lib/old_psimpl.h \
    ../lib/detail/util.h \
    ../lib/detail/math.h \
    ../lib/detail/algo.h
