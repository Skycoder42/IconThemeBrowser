TEMPLATE = app

QT       += core gui widgets concurrent

TARGET = iconthemebrowser
VERSION = 1.1.0

DEFINES += "TARGET=\\\"$$TARGET\\\""
DEFINES += "VERSION=\\\"$$VERSION\\\""
DEFINES += "COMPANY=\"\\\"Skycoder42\\\"\""
DEFINES += "DISPLAY_NAME=\"\\\"Icon Theme Browser\\\"\""

DEFINES += QT_DEPRECATED_WARNINGS

include(./vendor/vendor.pri)

HEADERS  += mainwindow.h \
	editpathsdialog.h

SOURCES += main.cpp\
		mainwindow.cpp \
	editpathsdialog.cpp

FORMS    += mainwindow.ui \
	editpathsdialog.ui

target.path = $$[QT_INSTALL_BINS]
INSTALLS += target
