include(../Common.pri)
include(../Platforms.pri)
include(../External.pri)
include(Sources.pri)

TARGET = $${APP_NAME}
TEMPLATE = app
# QT += opengl

# For now we don't build an Application bundle on Mac (comes later...)	
macx {
  # CONFIG -= app_bundle
}

DESTDIR = $$PWD/../../bin

macx {
    LIBS += -L$${DESTDIR}/$${APP_NAME}.app/Contents/Frameworks
} else {
    LIBS += -L$${DESTDIR}
}

LIBS += -lUtils \
        -lModel \
        -lResources \
        -lKernel

macx {
   ICON = res/ApplicationIcon.icns
}

win32 {
   OBJECTS_DIR = obj
   RC_FILE = res/Screenie.rc
}


