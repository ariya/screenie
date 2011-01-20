DEPENDPATH += $$PWD/../Utils/src \
              $$PWD/../Resources/src \
              $$PWD/../Model/src \
              $$PWD/GeneratedFiles \
              $$PWD/src/Clipboard \
              $$PWD/src

HEADERS += $$PWD/src/KernelLib.h \
           $$PWD/src/ExportImage.h \
           $$PWD/src/Reflection.h \
           $$PWD/src/ScreenieControl.h \
           $$PWD/src/ScreenieGraphicsScene.h \
           $$PWD/src/ScreeniePixmapItem.h \
           $$PWD/src/TemplateOrganizer.h \
           $$PWD/src/Clipboard/Clipboard.h \
           $$PWD/src/Clipboard/ScreenieMimeData.h \
           $$PWD/src/Clipboard/MimeHelper.h \
           $$PWD/src/PropertyDialogFactory.h \
           $$PWD/src/Dialogs/ScreenieModelPropertiesDialog.h \
           $$PWD/src/Dialogs/BaseGeometryPropertiesWidget.h \
           $$PWD/src/Dialogs/TemplateModelPropertiesWidget.h \
           $$PWD/src/Dialogs/TemplateModelPropertiesDialog.h \
           $$PWD/src/Dialogs/ReflectionPropertiesWidget.h

SOURCES += $$PWD/src/ExportImage.cpp \
           $$PWD/src/Reflection.cpp \
           $$PWD/src/ScreenieControl.cpp \
           $$PWD/src/ScreenieGraphicsScene.cpp \
           $$PWD/src/ScreeniePixmapItem.cpp \
           $$PWD/src/TemplateOrganizer.cpp \
           $$PWD/src/Clipboard/Clipboard.cpp \
           $$PWD/src/Clipboard/ScreenieMimeData.cpp \
           $$PWD/src/Clipboard/MimeHelper.cpp \
           $$PWD/src/PropertyDialogFactory.cpp \
           $$PWD/src/Dialogs/ScreenieModelPropertiesDialog.cpp \
           $$PWD/src/Dialogs/BaseGeometryPropertiesWidget.cpp \
           $$PWD/src/Dialogs/TemplateModelPropertiesWidget.cpp \
           $$PWD/src/Dialogs/TemplateModelPropertiesDialog.cpp \
           $$PWD/src/Dialogs/ReflectionPropertiesWidget.cpp

FORMS += $$PWD/ui/ScreenieModelPropertiesDialog.ui \
         $$PWD/ui/BaseGeometryPropertiesWidget.ui \
         $$PWD/ui/TemplateModelPropertiesWidget.ui \
         $$PWD/ui/ReflectionPropertiesWidget.ui
