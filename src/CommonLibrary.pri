include(Common.pri)

macx {
    QMAKE_LFLAGS_SONAME = -Wl,-install_name,@executable_path/../Frameworks/
}

macx {
    CONFIG(debug, debug|release) {
        DESTDIR = $$PWD/../bin/debug/$${APP_NAME}.app/Contents/Frameworks
        message(Building $$TARGET in debug mode)
    } else {
        DESTDIR = $$PWD/../bin/release/$${APP_NAME}.app/Contents/Frameworks
        message(Building $$TARGET in release mode)
    }
} else {
    CONFIG(debug, debug|release) {
        DESTDIR = $$PWD/../bin/debug
        message(Building $$TARGET in debug mode)
    } else {
        DESTDIR = $$PWD/../bin/release
        message(Building $$TARGET in release mode)
    }
}

