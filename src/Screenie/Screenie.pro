include(../Common.pri)
include(../External.pri)
include(Sources.pri)

TARGET = $${APP_NAME}
TEMPLATE = app

# OpenGL support later
# QT += opengl

CONFIG(debug, debug|release) {
    DESTDIR = $$PWD/../../bin/debug
    message(Building $$TARGET in debug mode)
} else {
    DESTDIR = $$PWD/../../bin/release
    message(Building $$TARGET in release mode)
}

macx {
    LIBS += -L$${DESTDIR}/$${APP_NAME}.app/Contents/Frameworks
} else {
    LIBS += -L$${DESTDIR}
}

LIBS += -lUtils$${VERSION_MAJ} \
        -lModel$${VERSION_MAJ} \
        -lResources$${VERSION_MAJ} \
        -lKernel$${VERSION_MAJ}

macx {
   ICON = res/ApplicationIcon.icns
   QMAKE_INFO_PLIST = Info.plist
}

win32 {
   RC_FILE = res/Screenie.rc
}


