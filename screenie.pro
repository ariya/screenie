TEMPLATE = app
TARGET = screenie
DEPENDPATH += .
INCLUDEPATH += .
FORMS = parameters.ui help.ui
SOURCES = screenie.cpp
#CONFIG += console
RESOURCES = resources/screenie.qrc
OBJECTS_DIR = .obj

macx {
   ICON = resources/screenie.icns
}

win32 {
   OBJECTS_DIR = obj
   RC_FILE = resources/screenie.rc
}

TRANSLATIONS = l10n/screenie_fr.ts \
               l10n/screenie_pl.ts \
               l10n/screenie_ru.ts

