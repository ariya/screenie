include(../CommonLibrary.pri)
include(../External.pri)
include(Sources.pri)

TEMPLATE = lib
DEFINES += MODEL_EXPORT

LIBS += -L$${DESTDIR} \
        -lUtils

