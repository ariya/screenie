DEPENDPATH += $$PWD/../Utils/src \
              $$PWD/../Resources/src \
              $$PWD/../Model/src \
              $$PWD/GeneratedFiles \
              $$PWD/src/Clipboard \
              $$PWD/src

HEADERS += $$PWD/src/ExportImage.h \
           $$PWD/src/Reflection.h \
           $$PWD/src/ScreenieControl.h \
           $$PWD/src/ScreenieGraphicsScene.h \
           $$PWD/src/ScreeniePixmapItem.h \
           $$PWD/src/KernelLib.h \
           $$PWD/src/Clipboard/Clipboard.h \
           $$PWD/src/Clipboard/ScreenieMimeData.h \
           $$PWD/src/Clipboard/MimeHelper.h

SOURCES += $$PWD/src/ExportImage.cpp \
           $$PWD/src/Reflection.cpp \
           $$PWD/src/ScreenieControl.cpp \
           $$PWD/src/ScreenieGraphicsScene.cpp \
           $$PWD/src/ScreeniePixmapItem.cpp \
           $$PWD/src/Clipboard/Clipboard.cpp \
           $$PWD/src/Clipboard/ScreenieMimeData.cpp \
           $$PWD/src/Clipboard/MimeHelper.cpp
