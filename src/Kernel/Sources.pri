DEPENDPATH += $$PWD/../Utils/src \
              $$PWD/../Resources/src \
              $$PWD/../Model/src \
              $$PWD/GeneratedFiles \
              $$PWD/src/Clipboard \
              $$PWD/src

HEADERS += $$PWD/src/ExportImage.h \
           $$PWD/src/Reflection.h \
           $$PWD/src/KernelLib.h \
           $$PWD/src/Clipboard/Clipboard.h \
    src/Clipboard/ScreenieMimeData.h

SOURCES += $$PWD/src/ExportImage.cpp \
           $$PWD/src/Reflection.cpp \
           $$PWD/src/Clipboard/Clipboard.cpp \
    src/Clipboard/ScreenieMimeData.cpp
