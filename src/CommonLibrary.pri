include(Common.pri)

!macx {
  CONFIG += plugin
}

macx {
    QMAKE_LFLAGS_SONAME = -Wl,-install_name,@executable_path/../Frameworks/
}

macx {
  DESTDIR = $$PWD/../bin/$${APP_NAME}.app/Contents/Frameworks
} else {
  DESTDIR = $$PWD/../bin/
}
