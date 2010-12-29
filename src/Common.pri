APP_NAME = Screenie

LANGUAGE = C++
CONFIG += qt warn_on thread

# On Mac we build 64bit Intel only
macx {
  CONFIG += x86_64
  CONFIG -= x86 ppc
}

DEFINES += QT_NO_COMPAT

CONFIG(debug, debug|release) {
    OBJECTS_DIR    = obj/debug
    MOC_DIR        = GeneratedFiles/debug
    UI_DIR         = GeneratedFiles/debug
    INCLUDEPATH   += GeneratedFiles/debug
    DEFINES += DEBUG
} else {
    OBJECTS_DIR    = obj/release
    MOC_DIR        = GeneratedFiles/release
    UI_DIR         = GeneratedFiles/release
    INCLUDEPATH   += GeneratedFiles/release
}
