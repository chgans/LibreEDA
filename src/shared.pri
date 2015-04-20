DESTDIR = $$OUT_PWD/..
LIBS += -L$$OUT_PWD/
INCLUDEPATH += $$PWD

# Warn when a literal '0' is used as null pointer constant. This can be useful
# to facilitate the conversion to nullptr in C++11. (GCC)
# => Would have been nice, but Qt headers are full of these...
# QMAKE_CXXFLAGS += -Wzero-as-null-pointer-constant
