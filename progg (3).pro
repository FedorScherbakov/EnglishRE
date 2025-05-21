QT += widgets sql
TARGET = progg
CONFIG += c++11

SOURCES += main.cpp \
           databasemanager.cpp \
           testapp.cpp \
           logowidget.cpp \
           authdialog.cpp

HEADERS += databasemanager.h \
           testapp.h \
           logowidget.h \
           authdialog.h
