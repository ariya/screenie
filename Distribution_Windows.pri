#include(Common.pri)

QT_BIN_DIR = $$[QT_INSTALL_BINS]
QT_BIN_DIR = $$replace(QT_BIN_DIR, /, \\)
QT_PLUGINS_DIR = $$[QT_INSTALL_PLUGINS]
QT_PLUGINS_DIR = $$replace(QT_PLUGINS_DIR, /, \\)

CONFIG(debug, debug|release) {
    BIN_DIR = bin\\debug
    DIST_DIR = dist
    QT_CORE = QtCored4.dll
    QT_GUI = QtGuid4.dll
    QT_IMAGE_JPEG = qjpegd4.dll
    QT_IMAGE_TIFF = qtiffd4.dll
    QT_IMAGE_GIF = qgifd4.dll
    message(Distribution: DEBUG $${APP_NAME} with $${QT_BIN_DIR} and $${QT_PLUGINS_DIR})
} else {
    BIN_DIR = bin\\release
    DIST_DIR = dist
    QT_CORE = QtCore4.dll
    QT_GUI = QtGui4.dll
    QT_IMAGE_JPEG = qjpeg4.dll
    QT_IMAGE_TIFF = qtiff4.dll
    QT_IMAGE_GIF = qgif4.dll
    message(Distribution: RELEASE $${APP_NAME} with $${QT_BIN_DIR} and $${QT_PLUGINS_DIR})
}

#
# Distribution
#

# Remove previous dist
distribution.commands += rmdir /S /Q "$${DIST_DIR}" &
distribution.commands += mkdir "$${DIST_DIR}" &&

# Copy application libraries from 'bin' to 'dist'
distribution.commands += xcopy "$${BIN_DIR}\\*$${VERSION_MAJ}.dll" "$${DIST_DIR}" &&

# Copy application binary from 'bin' to 'dist'
distribution.commands += xcopy "$${BIN_DIR}\\$${APP_NAME}.exe" "$${DIST_DIR}" &&

# Copy Qt libraries
distribution.commands += xcopy "$${QT_BIN_DIR}\\$${QT_CORE}" "$${DIST_DIR}" &&
distribution.commands += xcopy "$${QT_BIN_DIR}\\$${QT_GUI}" "$${DIST_DIR}" &&
distribution.commands += xcopy "$${QT_BIN_DIR}\\libgcc_s_dw2-1.dll" "$${DIST_DIR}" &&
distribution.commands += xcopy "$${QT_BIN_DIR}\\mingwm10.dll" "$${DIST_DIR}" &&

# Copy Qt plugins
distribution.commands += mkdir "$${DIST_DIR}\\imageformats" &&
distribution.commands += xcopy "$${QT_PLUGINS_DIR}\\imageformats\\$${QT_IMAGE_JPEG}" "$${DIST_DIR}\\imageformats" &&
distribution.commands += xcopy "$${QT_PLUGINS_DIR}\\imageformats\\$${QT_IMAGE_TIFF}" "$${DIST_DIR}\\imageformats" &&
distribution.commands += xcopy "$${QT_PLUGINS_DIR}\\imageformats\\$${QT_IMAGE_GIF}" "$${DIST_DIR}\\imageformats"

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
  installer.commands = candle.exe -nologo "Installer\\Windows\\Screenie.wxs" -out "Installer\\Windows\\Screenie.wixobj"  -ext WixUIExtension &&
  installer.commands += light.exe -nologo "Installer\\Windows\\Screenie.wixobj" -out "Installer\\Windows\\Screenie.msi"  -ext WixUIExtension
}


