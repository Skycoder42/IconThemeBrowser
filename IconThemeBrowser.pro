TEMPLATE = app

QT       += core gui widgets concurrent

TARGET = IconThemeBrowser
VERSION = 1.0.0

DEFINES += "TARGET=\\\"$$TARGET\\\""
DEFINES += "VERSION=\\\"$$VERSION\\\""
DEFINES += "COMPANY=\"\\\"Skycoder42\\\"\""
DEFINES += "DISPLAY_NAME=\"\\\"Icon Theme Browser\\\"\""

DEFINES += QT_DEPRECATED_WARNINGS

include(./DialogMaster/dialogmaster.pri)

HEADERS  += mainwindow.h \
	editpathsdialog.h

SOURCES += main.cpp\
		mainwindow.cpp \
	editpathsdialog.cpp

FORMS    += mainwindow.ui \
	editpathsdialog.ui
