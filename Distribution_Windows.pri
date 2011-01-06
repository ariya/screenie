#include(Common.pri)

CONFIG(debug, debug|release) {
    BIN_DIR = bin\\debug
    DIST_DIR = dist
    QT_CORE = QtCored4.dll
    QT_GUI = QtGuid4.dll
    QT_IMAGE_JPEG = qjpegd4.dll
    QT_IMAGE_TIFF = qtiffd4.dll
    QT_IMAGE_GIF = qgifd4.dll
    message(Distributing $${APP_NAME} together with $$(QTDIR) in debug mode)
} else {
    BIN_DIR = bin\\release
    DIST_DIR = dist
    QT_CORE = QtCore4.dll
    QT_GUI = QtGui4.dll
    QT_IMAGE_JPEG = qjpeg4.dll
    QT_IMAGE_TIFF = qtiff4.dll
    QT_IMAGE_GIF = qgif4.dll
    message(Distributing $${APP_NAME} together with $$(QTDIR) in release mode)
}

# Remove previous dist
distribution.commands += rmdir /S /Q $${DIST_DIR} &
distribution.commands += mkdir $${DIST_DIR} &&

# Copy application libraries from 'bin' to 'dist'
distribution.commands += xcopy $${BIN_DIR}\\*$${VERSION_MAJ}.dll $${DIST_DIR} &&

# Copy application binary from 'bin' to 'dist'
distribution.commands += xcopy $${BIN_DIR}\\$${APP_NAME}.exe $${DIST_DIR} &&

# Copy Qt libraries
distribution.commands += xcopy $$(QTDIR)\\bin\\$${QT_CORE} $${DIST_DIR} &&
distribution.commands += xcopy $$(QTDIR)\\bin\\$${QT_GUI} $${DIST_DIR} &&
distribution.commands += xcopy $$(QTDIR)\\bin\\libgcc_s_dw2-1.dll $${DIST_DIR} &&
distribution.commands += xcopy $$(QTDIR)\\bin\\mingwm10.dll $${DIST_DIR} &&

# Copy Qt plugins
distribution.commands += mkdir $${DIST_DIR}\\imageformats &&
distribution.commands += xcopy $$(QTDIR)\\plugins\\imageformats\\$${QT_IMAGE_JPEG} $${DIST_DIR}\\imageformats &&
distribution.commands += xcopy $$(QTDIR)\\plugins\\imageformats\\$${QT_IMAGE_TIFF} $${DIST_DIR}\\imageformats &&
distribution.commands += xcopy $$(QTDIR)\\plugins\\imageformats\\$${QT_IMAGE_GIF} $${DIST_DIR}\\imageformats

