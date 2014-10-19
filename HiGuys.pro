QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HiGuys
TEMPLATE = app

SOURCES += main.cpp \
    periodiccameracapture.cpp \
    fileuploader.cpp \
    fileupload.cpp \
    settingsdialog.cpp \
    trayicon.cpp

HEADERS  += \
    periodiccameracapture.h \
    fileuploader.h \
    fileupload.h \
    settingsdialog.h \
    trayicon.h

RESOURCES += \
    resources.qrc
