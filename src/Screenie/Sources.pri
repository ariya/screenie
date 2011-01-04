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
           $$PWD/src/MainWindow.h \
           $$PWD/src/ScreenieApplication.h

SOURCES += $$PWD/src/Main.cpp \
           $$PWD/src/MainWindow.cpp \
           $$PWD/src/ScreenieApplication.cpp

FORMS += $$PWD/ui/MainWindow.ui
