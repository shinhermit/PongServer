######################################################################
# Automatically generated by qmake (2.01a) Fri Mar 29 00:47:42 2013
######################################################################

TEMPLATE = app
TARGET = 
DEPENDPATH += . headers sources tests
INCLUDEPATH += . headers
QT += network

# Input
HEADERS += headers/PlayingArea.hpp \
           headers/PongTypes.hpp \
           headers/Workers.hpp \
    headers/Trigo.hpp \
    headers/Random.hpp \
    headers/PlayerState.hpp \
    headers/GameState.hpp \
    headers/GameStateWorker.hpp \
    headers/LoggerWorker.hpp \
    headers/SocketWorker.hpp \
    headers/Lockable.hpp
SOURCES += sources/PlayingArea.cpp \
           tests/test_playing_area.cpp \
    sources/Trigo.cpp \
    sources/Random.cpp \
    sources/PlayerState.cpp \
    sources/GameState.cpp \
    sources/GameStateWorker.cpp \
    sources/LoggerWorker.cpp \
    sources/Lockable.cpp \
    sources/SocketWorker.cpp

OTHER_FILES +=
