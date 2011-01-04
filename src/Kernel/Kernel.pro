include(../CommonLibrary.pri)
include(../External.pri)
include(Sources.pri)

TEMPLATE = lib
DEFINES += KERNEL_EXPORT

LIBS += -L$${DESTDIR} \
        -lUtils \
        -lModel


