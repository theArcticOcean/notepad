QT       += core gui xml
TRANSLATIONS += English.ts Chinese.ts
MOC_DIR = ./build/moc_cpp
OBJECTS_DIR = ./build

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

HEADERS += \
    Inc/textreplace.h \
    Inc/textfind.h \
    Inc/simlighter.h \
    Inc/regLighter.h \
    Inc/notepad.h \
    Inc/handler.h \
    Inc/charmap.h \
    Inc/xmlrecorder.h

SOURCES += \
    Src/xmlrecorder.cpp \
    Src/textreplace.cpp \
    Src/textfind.cpp \
    Src/simlighter.cpp \
    Src/regLighter.cpp \
    Src/notepad.cpp \
    Src/main.cpp \
    Src/handler.cpp \
    Src/charmap.cpp

RESOURCES += \
    image.qrc
