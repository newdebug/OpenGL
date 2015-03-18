#-------------------------------------------------
#
# Project created by QtCreator 2013-10-19T02:08:03
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenGLViewer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    GLWidget.cpp

HEADERS  += mainwindow.h \
    GLWidget.h

FORMS    += mainwindow.ui
