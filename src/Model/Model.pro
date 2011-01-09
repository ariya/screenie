include(../CommonLibrary.pri)
include(../External.pri)
include(Sources.pri)

TEMPLATE = lib
DEFINES += MODEL_EXPORT

win32 {
  LIBS += -L$${DESTDIR} \
          -lUtils$${VERSION_MAJ}
} else {
  LIBS += -L$${DESTDIR} \
          -lUtils
}


