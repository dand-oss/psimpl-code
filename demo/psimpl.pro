TARGET = psimpl-demo
TEMPLATE = app
VERSION = 7
SOURCES += main.cpp \
    MainWindow.cpp \
    DPWorker.cpp \
    RenderArea.cpp
HEADERS += MainWindow.h \
    DPWorker.h \
    RenderArea.h \
    psimpl_reference.h \
    psimpl.h \
    ../lib/psimpl.h
FORMS += MainWindow.ui
OTHER_FILES += \
    resource.rc \
    ../README.txt \
    ../LICENSE.txt
RC_FILE = resource.rc

RESOURCES += \
    icons.qrc
