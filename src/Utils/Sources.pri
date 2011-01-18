DEPENDPATH += $$PWD/GeneratedFiles \
              $$PWD/src

HEADERS += $$PWD/src/UtilsLib.h \
           $$PWD/src/PaintTools.h \
           $$PWD/src/Settings.h \
           $$PWD/src/Version.h \
           $$PWD/src/SizeFitter.h \
           $$PWD/src/FileUtils.h

SOURCES += $$PWD/src/PaintTools.cpp \
           $$PWD/src/Settings.cpp \
           $$PWD/src/Version.cpp \
           $$PWD/src/SizeFitter.cpp \
           $$PWD/src/FileUtils.cpp

unix {
  SOURCES += $$PWD/src/FileUtils_Unix.cpp
}

win32 {
  SOURCES += $$PWD/src/FileUtils_Windows.cpp
}
