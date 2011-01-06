include(src/Common.pri)
macx {
  include(Distribution_Mac.pri)
}

distribution.depends = all

QMAKE_EXTRA_TARGETS += distribution
