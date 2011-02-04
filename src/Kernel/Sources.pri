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
           $$PWD/src/DocumentManager.h \
           $$PWD/src/DocumentInfo.h \
           $$PWD/src/Dialogs/ScreenieModelPropertiesDialog.h \
           $$PWD/src/Dialogs/GeometryPropertiesWidget.h \
           $$PWD/src/Dialogs/TemplateModelPropertiesWidget.h \
           $$PWD/src/Dialogs/TemplateModelPropertiesDialog.h \
           $$PWD/src/Dialogs/ReflectionPropertiesWidget.h \
           $$PWD/src/Dialogs/FilePathModelPropertiesDialog.h \
           $$PWD/src/Dialogs/FilePathModelPropertiesWidget.h \
           $$PWD/src/Dialogs/ImageModelPropertiesDialog.h

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
           $$PWD/src/DocumentManager.cpp \
           $$PWD/src/Dialogs/ScreenieModelPropertiesDialog.cpp \
           $$PWD/src/Dialogs/GeometryPropertiesWidget.cpp \
           $$PWD/src/Dialogs/TemplateModelPropertiesWidget.cpp \
           $$PWD/src/Dialogs/TemplateModelPropertiesDialog.cpp \
           $$PWD/src/Dialogs/ReflectionPropertiesWidget.cpp \
           $$PWD/src/Dialogs/FilePathModelPropertiesDialog.cpp \
           $$PWD/src/Dialogs/FilePathModelPropertiesWidget.cpp \
           $$PWD/src/Dialogs/ImageModelPropertiesDialog.cpp

FORMS += $$PWD/ui/ScreenieModelPropertiesDialog.ui \
         $$PWD/ui/GeometryPropertiesWidget.ui \
         $$PWD/ui/TemplateModelPropertiesWidget.ui \
         $$PWD/ui/ReflectionPropertiesWidget.ui \
         $$PWD/ui/FilePathModelPropertiesWidget.ui
