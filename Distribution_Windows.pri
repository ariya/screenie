#include(Common.pri)

QT_BIN_DIR = $$[QT_INSTALL_BINS]
QT_PLUGINS_DIR = $$[QT_INSTALL_PLUGINS]

CONFIG(debug, debug|release) {
    DIST_DIR = dist
    QT_CORE = QtCored4.dll
    QT_GUI = QtGuid4.dll
    QT_IMAGE_JPEG = qjpegd4.dll
    QT_IMAGE_TIFF = qtiffd4.dll
    QT_IMAGE_GIF = qgifd4.dll
    message(Installer: DEBUG $${APP_NAME} with $${QT_BIN_DIR} and $${QT_PLUGINS_DIR})
} else {
    DIST_DIR = dist
    QT_CORE = QtCore4.dll
    QT_GUI = QtGui4.dll
    QT_IMAGE_JPEG = qjpeg4.dll
    QT_IMAGE_TIFF = qtiff4.dll
    QT_IMAGE_GIF = qgif4.dll
    message(Installer: RELEASE $${APP_NAME} with $${QT_BIN_DIR} and $${QT_PLUGINS_DIR})
}

#
# INSTALLS
#

qt_lib.files = "$${QT_BIN_DIR}/$${QT_CORE}" "$${QT_BIN_DIR}/$${QT_GUI}" "$${QT_BIN_DIR}/libgcc_s_dw2-1.dll" "$${QT_BIN_DIR}/mingwm10.dll"
qt_lib.path = "$${DIST_DIR}"

qt_img_plugins.files = "$${QT_PLUGINS_DIR}/imageformats/$${QT_IMAGE_JPEG}" "$${QT_PLUGINS_DIR}/imageformats/$${QT_IMAGE_TIFF}" "$${QT_PLUGINS_DIR}/imageformats/$${QT_IMAGE_GIF}"
qt_img_plugins.path = "$${DIST_DIR}/imageformats"

INSTALLS += qt_lib qt_img_plugins

#
# Installer
#

# Make sure the WiX installer tools candle.exe and light.exe are in the PATH
# e.g. set PATH="c:\Program Files\WixEdit\wix-3.0.5419.0";%PATH%
# Attention: with the current version ming32-make crashes when the WiX tools
# are APPENDED to the PATH. It seems to work when PREPENDING to the PATH!
CONFIG(debug, debug|release) {
  message("Installer: skipping this build step. Target 'Installer' can only be done in RELEASE mode.")
} else {
  installer.commands = candle.exe -nologo "Installer/Windows/Screenie.wxs" -out "Installer/Windows/Screenie.wixobj"  -ext WixUIExtension &&
  installer.commands += light.exe -nologo "Installer/Windows/Screenie.wixobj" -out "Installer/Windows/Screenie-$${VERSION}.msi"  -ext WixUIExtension
}


