# *********************************
# platform dependend configurations
#

unix:linux* {
    QMAKE_CXXFLAGS += -Wall -Woverloaded-virtual -Wno-deprecated -Wuninitialized -O
    QMAKE_CFLAGS += -Wstrict-prototypes -Wmissing-prototypes

    QMAKE_LFLAGS += -Wl,--rpath=\'\$\$ORIGIN\'
}

win32 {
    QMAKE_CXXFLAGS += 
    QMAKE_CFLAGS +=
}

macx {
    QMAKE_CXXFLAGS += -Wall -Woverloaded-virtual -Wno-deprecated -Wuninitialized -O
    QMAKE_CFLAGS += -Wstrict-prototypes -Wmissing-prototypes
}

