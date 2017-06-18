TEMPLATE = app

QT       += core gui widgets concurrent

TARGET = iconthemebrowser
VERSION = 1.0.0

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

inbin.path = /usr/bin
inbin.files = $$TARGET
indsk.path = /usr/share/applications/
indsk.files = de.skycoder42.$${TARGET}.desktop

INSTALLS += inbin indsk
