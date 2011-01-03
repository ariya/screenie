include(Common.pri)

# Actually we want prevent qmake from generating symbolic links (Unix, Mac)
# and adding the version number into the library file name (Unix, Windows, Mac)
# by putting the 'plugin' keyword into the CONFIG.
# But on Mac this apparently also prevents the the install_name_tool (?)
# from being applied on the generated library.
# But since the libraries are put into the application bundle anyway
# (which is easy to be moved around in the file system) this is not so bad.
!macx {
  CONFIG += plugin
}

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
