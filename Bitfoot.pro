TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += .
INCLUDEPATH += src
INCLUDEPATH += senjo

unix:QMAKE_CXXFLAGS += -std=c++11
unix:LIBS += -lpthread

CONFIG(release, debug|release) {
  message(Release build!)
  DEFINES += NDEBUG
}

# deploy epd files with each build
include(epd.pri)

# add GITREV to QMAKE_CXXFLAGS
include(gitrev.pri)

SOURCES += \
    src/HashTable.cpp \
    src/Stats.cpp \
    src/Bitfoot.cpp \
    src/main.cpp \
    senjo/BackgroundCommand.cpp \
    senjo/ChessEngine.cpp \
    senjo/EngineOption.cpp \
    senjo/MoveFinder.cpp \
    senjo/Output.cpp \
    senjo/Threading.cpp \
    senjo/UCIAdapter.cpp

HEADERS += \
    src/Defs.h \
    src/Diff.h \
    src/HashTable.h \
    src/Move.h \
    src/Stats.h \
    src/Bitfoot.h \
    senjo/BackgroundCommand.h \
    senjo/ChessEngine.h \
    senjo/ChessMove.h \
    senjo/EngineOption.h \
    senjo/MoveFinder.h \
    senjo/Output.h \
    senjo/Platform.h \
    senjo/Square.h \
    senjo/Threading.h \
    senjo/UCIAdapter.h

DISTFILES += \
    epd/*.epd
