include(../CommonLibrary.pri)
include(../External.pri)
include(Sources.pri)

TEMPLATE = lib
DEFINES += KERNEL_EXPORT

win32 {
  LIBS += -L$${DESTDIR} \
          -lUtils$${VERSION_MAJ} \
          -lModel$${VERSION_MAJ} \
          -lResources$${VERSION_MAJ}
} else {
  LIBS += -L$${DESTDIR} \
          -lUtils \
          -lModel \
          -lResources
}


