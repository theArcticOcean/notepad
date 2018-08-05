QT       += core gui xml

macx{
    message("compile for mac os x")
    ICON = image/logo.icns
}

win32{
    message("compile for windows")
    RC_FILE = res.rc
}

INCLUDEPATH += "Inc/"

TARGET = myNote
VERSION = 1.0
MOC_DIR = ./build/moc_cpp
OBJECTS_DIR = ./build
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

HEADERS += \
    Inc/textreplace.h \
    Inc/textfind.h \
    Inc/simlighter.h \
    Inc/notepad.h \
    Inc/handler.h \
    Inc/charmap.h \
    Inc/xmlrecorder.h \
    Inc/log.h \
    Inc/commondata.h \
    Inc/reglighter.h

SOURCES += \
    Src/xmlrecorder.cpp \
    Src/textreplace.cpp \
    Src/textfind.cpp \
    Src/simlighter.cpp \
    Src/notepad.cpp \
    Src/main.cpp \
    Src/handler.cpp \
    Src/charmap.cpp \
    Src/log.cpp \
    Src/reglighter.cpp

RESOURCES += \
    image.qrc

DISTFILES +=
