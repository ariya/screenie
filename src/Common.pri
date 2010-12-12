APP_NAME = Screenie

LANGUAGE       = C++
CONFIG        += qt warn_on thread

# On Mac we build 64bit Intel only
macx {
  CONFIG      += x86_64
  CONFIG      -= x86 ppc
}

DEFINES       += QT_NO_COMPAT
OBJECTS_DIR    = obj
MOC_DIR        = GeneratedFiles
UI_DIR         = GeneratedFiles
INCLUDEPATH   += GeneratedFiles
debug:DEFINES += DEBUG
