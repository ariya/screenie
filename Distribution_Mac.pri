#include(Common.pri)

CONFIG(debug, debug|release) {
    APP_BUNDLE = bin/debug/$${APP_NAME}.app
    message(Distributing $$APP_NAME in debug mode)
} else {
    APP_BUNDLE = bin/release/$${APP_NAME}.app
    message(Distributing $$APP_NAME in release mode)
}
DIST_APP_BUNDLE = dist/$${APP_NAME}.app
FRAMEWORKS_DIR = $${DIST_APP_BUNDLE}/Contents/Frameworks
QT_PLUGINS_DIR = $${DIST_APP_BUNDLE}/Contents/PlugIns

distribution.commands += @echo Making dist on Mac;

# Remove previous bundle
distribution.commands += test -d $$DIST_APP_BUNDLE && rm -rf $$DIST_APP_BUNDLE;

distribution.commands += test -d dist || mkdir dist fi;
distribution.commands += cp -R ./$$APP_BUNDLE ./$$DIST_APP_BUNDLE;

# Qt Core
distribution.commands += test -d $$FRAMEWORKS_DIR/QtCore.framework || mkdir $$FRAMEWORKS_DIR/QtCore.framework;
distribution.commands += test -d $$FRAMEWORKS_DIR/QtCore.framework/Versions || mkdir $$FRAMEWORKS_DIR/QtCore.framework/Versions;
distribution.commands += test -d $$FRAMEWORKS_DIR/QtCore.framework/Versions/4 || mkdir $$FRAMEWORKS_DIR/QtCore.framework/Versions/4;
distribution.commands += cp /Library/Frameworks/QtCore.framework/QtCore $$FRAMEWORKS_DIR/QtCore.framework/Versions/4;

distribution.commands += install_name_tool -id @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore $$DIST_APP_BUNDLE/Contents/Frameworks/QtCore.framework/Versions/4/QtCore;

# Qt Gui
distribution.commands += test -d $$FRAMEWORKS_DIR/QtGui.framework || mkdir $$FRAMEWORKS_DIR/QtGui.framework;
distribution.commands += test -d $$FRAMEWORKS_DIR/QtGui.framework/Resources || mkdir $$FRAMEWORKS_DIR/QtGui.framework/Resources;
distribution.commands += test -d $$FRAMEWORKS_DIR/QtGui.framework/Versions || mkdir $$FRAMEWORKS_DIR/QtGui.framework/Versions;
distribution.commands += test -d $$FRAMEWORKS_DIR/QtGui.framework/Versions/4 || mkdir $$FRAMEWORKS_DIR/QtGui.framework/Versions/4;
distribution.commands += cp -R /Library/Frameworks/QtGui.framework/Resources/qt_menu.nib $$FRAMEWORKS_DIR/QtGui.framework/Resources/qt_menu.nib;
distribution.commands += cp /Library/Frameworks/QtGui.framework/QtGui $$FRAMEWORKS_DIR/QtGui.framework/Versions/4;

distribution.commands += install_name_tool -id @executable_path/../Frameworks/QtGui.framework/Versions/4/QtGui $$DIST_APP_BUNDLE/Contents/Frameworks/QtGui.framework/Versions/4/QtGui;
distribution.commands += install_name_tool -change QtCore.framework/Versions/4/QtCore \
                                                   @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore \
                                                   $$DIST_APP_BUNDLE/Contents/Frameworks/QtGui.framework/Versions/4/QtGui;

# Utils library
distribution.commands += install_name_tool -change QtCore.framework/Versions/4/QtCore \
                                                   @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore \
                                                   $$FRAMEWORKS_DIR/libUtils.$${VERSION}.dylib;
distribution.commands += install_name_tool -change QtGui.framework/Versions/4/QtGui \
                                                   @executable_path/../Frameworks/QtGui.framework/Versions/4/QtGui \
                                                   $$FRAMEWORKS_DIR/libUtils.$${VERSION}.dylib;

# Resources library
distribution.commands += install_name_tool -change QtCore.framework/Versions/4/QtCore \
                                                   @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore \
                                                   $$FRAMEWORKS_DIR/libResources.$${VERSION}.dylib;
distribution.commands += install_name_tool -change QtGui.framework/Versions/4/QtGui \
                                                   @executable_path/../Frameworks/QtGui.framework/Versions/4/QtGui \
                                                   $$FRAMEWORKS_DIR/libResources.$${VERSION}.dylib;

# Model library
distribution.commands += install_name_tool -change QtCore.framework/Versions/4/QtCore \
                                                   @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore \
                                                   $$FRAMEWORKS_DIR/libModel.$${VERSION}.dylib;
distribution.commands += install_name_tool -change QtGui.framework/Versions/4/QtGui \
                                                   @executable_path/../Frameworks/QtGui.framework/Versions/4/QtGui \
                                                   $$FRAMEWORKS_DIR/libModel.$${VERSION}.dylib;

# Kernel library
distribution.commands += install_name_tool -change QtCore.framework/Versions/4/QtCore \
                                                   @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore \
                                                   $$FRAMEWORKS_DIR/libKernel.$${VERSION}.dylib;
distribution.commands += install_name_tool -change QtGui.framework/Versions/4/QtGui \
                                                   @executable_path/../Frameworks/QtGui.framework/Versions/4/QtGui \
                                                   $$FRAMEWORKS_DIR/libKernel.$${VERSION}.dylib;

# Application executable
distribution.commands += install_name_tool -change QtCore.framework/Versions/4/QtCore \
                                                   @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore \
                                                   $$DIST_APP_BUNDLE/Contents/MacOS/$${APP_NAME};
distribution.commands += install_name_tool -change QtGui.framework/Versions/4/QtGui \
                                                   @executable_path/../Frameworks/QtGui.framework/Versions/4/QtGui \
                                                   $$DIST_APP_BUNDLE/Contents/MacOS/$${APP_NAME};

# Qt Plugins
distribution.commands += test -d $$QT_PLUGINS_DIR || mkdir $$QT_PLUGINS_DIR;
distribution.commands += test -d $$QT_PLUGINS_DIR/imageformats || mkdir $$QT_PLUGINS_DIR/imageformats;

# Image JPEG plugin
distribution.commands += cp -R /Developer/Applications/Qt/plugins/imageformats/libqjpeg.dylib $$QT_PLUGINS_DIR/imageformats;
distribution.commands += install_name_tool -change QtCore.framework/Versions/4/QtCore \
                                                   @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore \
                                                   $$QT_PLUGINS_DIR/imageformats/libqjpeg.dylib;
distribution.commands += install_name_tool -change QtGui.framework/Versions/4/QtGui \
                                                   @executable_path/../Frameworks/QtGui.framework/Versions/4/QtGui \
                                                   $$QT_PLUGINS_DIR/imageformats/libqjpeg.dylib;

# Image TIFF plugin
distribution.commands += cp -R /Developer/Applications/Qt/plugins/imageformats/libqtiff.dylib $$QT_PLUGINS_DIR/imageformats;
distribution.commands += install_name_tool -change QtCore.framework/Versions/4/QtCore \
                                                   @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore \
                                                   $$QT_PLUGINS_DIR/imageformats/libqtiff.dylib;
distribution.commands += install_name_tool -change QtGui.framework/Versions/4/QtGui \
                                                   @executable_path/../Frameworks/QtGui.framework/Versions/4/QtGui \
                                                   $$QT_PLUGINS_DIR/imageformats/libqtiff.dylib;

# Image GIF plugin
distribution.commands += cp -R /Developer/Applications/Qt/plugins/imageformats/libqgif.dylib $$QT_PLUGINS_DIR/imageformats;
distribution.commands += install_name_tool -change QtCore.framework/Versions/4/QtCore \
                                                   @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore \
                                                   $$QT_PLUGINS_DIR/imageformats/libqgif.dylib;
distribution.commands += install_name_tool -change QtGui.framework/Versions/4/QtGui \
                                                   @executable_path/../Frameworks/QtGui.framework/Versions/4/QtGui \
                                                   $$QT_PLUGINS_DIR/imageformats/libqgif.dylib;

# qt.conf
distribution.commands += echo \"[Paths]\\nPlugins = PlugIns\" | cat > $$DIST_APP_BUNDLE/Contents/Resources/qt.conf;

# Strip unused architecture

!x86 {
  STRIP_ARCH=i386
}

!x86_64 {
  STRIP_ARCH=x86_64
}

message(Stripping $$STRIP_ARCH arch)
distribution.commands += lipo -remove $$STRIP_ARCH $$FRAMEWORKS_DIR/QtCore.framework/Versions/4/QtCore -output $$FRAMEWORKS_DIR/QtCore.framework/Versions/4/QtCore;
distribution.commands += lipo -remove i386 $$FRAMEWORKS_DIR/QtGui.framework/Versions/4/QtGui -output $$FRAMEWORKS_DIR/QtGui.framework/Versions/4/QtGui;
distribution.commands += lipo -remove i386 $$QT_PLUGINS_DIR/imageformats/libqjpeg.dylib -output $$QT_PLUGINS_DIR/imageformats/libqjpeg.dylib;
distribution.commands += lipo -remove i386 $$QT_PLUGINS_DIR/imageformats/libqtiff.dylib -output $$QT_PLUGINS_DIR/imageformats/libqtiff.dylib;
distribution.commands += lipo -remove i386 $$QT_PLUGINS_DIR/imageformats/libqgif.dylib -output $$QT_PLUGINS_DIR/imageformats/libqgif.dylib;




