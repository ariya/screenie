DEPENDPATH += $$PWD/../Utils/src \
              $$PWD/../Resources/src \
              $$PWD/../Model/src \
              $$PWD/../Model/src/Dao \
              $$PWD/../Model/src/Dao/Xml \
              $$PWD/../Kernel/src \
              $$PWD/../Kernel/src/Clipboard \
              $$PWD/UserInterface/src \
              $$PWD/GeneratedFiles \
              $$PWD/src

HEADERS += $$PWD/src/Main.h \
           $$PWD/UserInterface/src/MainWindow.h \
           $$PWD/UserInterface/src/ScreenieControl.h \
           $$PWD/UserInterface/src/ScreenieGraphicsScene.h \
           $$PWD/UserInterface/src/ScreeniePixmapItem.h

SOURCES += $$PWD/src/Main.cpp \
           $$PWD/UserInterface/src/MainWindow.cpp \
           $$PWD/UserInterface/src/ScreenieControl.cpp \
           $$PWD/UserInterface/src/ScreenieGraphicsScene.cpp \
           $$PWD/UserInterface/src/ScreeniePixmapItem.cpp

FORMS += $$PWD/UserInterface/ui/MainWindow.ui

