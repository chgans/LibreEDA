DESTDIR = $$OUT_PWD/..
LIBS += -L$$OUT_PWD/
INCLUDEPATH += $$PWD

CONFIG += c++11

BASE_DATA_DIR = $$PWD/../data
BASE_SETTINGS_DIR = $${BASE_DATA_DIR}/settings
BASE_SAMPLE_DIR =$${BASE_DATA_DIR}/samples

DEFINES += BASE_DATA_DIR=\\\"$${BASE_DATA_DIR}\\\" BASE_SETTINGS_DIR=\\\"$${BASE_SETTINGS_DIR}\\\" BASE_SAMPLE_DIR=\\\"$${BASE_SAMPLE_DIR}\\\"

# Warn when a literal '0' is used as null pointer constant. This can be useful
# to facilitate the conversion to nullptr in C++11. (GCC)
# => Would have been nice, but Qt headers are full of these...
# QMAKE_CXXFLAGS += -Wzero-as-null-pointer-constant
