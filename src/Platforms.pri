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

