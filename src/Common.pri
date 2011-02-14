APP_NAME = Screenie

LANGUAGE = C++
CONFIG += qt warn_on thread

# Should match with Utils/src/Version.h
VERSION=1.0.0

# qmake variable VER_MAJ seems to be empty, so
# applying the following "hack", inspired by:
# http://www.qtcentre.org/threads/17984-qmake-how-to-extract-number-from-variable
VERSIONS = $$split(VERSION, ".")
VERSION_MAJ = $$member(VERSIONS, 0)
VERSION_MIN = $$member(VERSIONS, 1)

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

target.path = $$PWD/../dist
INSTALLS += target

unix:linux* {
    QMAKE_CXXFLAGS += -Wall -Woverloaded-virtual -Wno-deprecated -Wuninitialized -O
    QMAKE_CFLAGS += -Wstrict-prototypes -Wmissing-prototypes

    QMAKE_LFLAGS += -Wl,--rpath=\'\$\$ORIGIN\'
}

win32-g++ {
    QMAKE_CXXFLAGS += -Wall -Woverloaded-virtual -Wno-deprecated -Wuninitialized -O
    QMAKE_CFLAGS += -Wstrict-prototypes -Wmissing-prototypes
}

macx {
    QMAKE_CXXFLAGS += -Wall -Woverloaded-virtual -Wno-deprecated -Wuninitialized -O
    QMAKE_CFLAGS += -Wstrict-prototypes -Wmissing-prototypes
}

