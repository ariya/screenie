include(src/Common.pri)
macx {
  include(Distribution_Mac.pri)
}
win32 {
  include(Distribution_Windows.pri)
}

distribution.depends = all

QMAKE_EXTRA_TARGETS += distribution
