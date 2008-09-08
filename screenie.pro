TEMPLATE = app
TARGET = screenie
DEPENDPATH += .
INCLUDEPATH += .
FORMS = parameters.ui help.ui
SOURCES = screenie.cpp
#CONFIG += console
RESOURCES = screenie.qrc

macx {
   ICON = screenie.icns
}

win32 {
   RC_FILE = screenie.rc
}

