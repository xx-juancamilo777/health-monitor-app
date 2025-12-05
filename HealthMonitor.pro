QT += core gui widgets sql
TARGET = HealthMonitor
TEMPLATE = app
CONFIG += c++11
SOURCES += src/main.cpp
HEADERS += include/DatabaseManager.h
LIBS += -lsqlite3
unix {
    QMAKE_CXXFLAGS += -std=c++11
}
