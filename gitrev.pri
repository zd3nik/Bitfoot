# include this pri to automaticall #define GIT_REV
unix:QMAKE_CXXFLAGS += -DGIT_REV=$$system(cat .git/refs/heads/master)
win32:QMAKE_CXXFLAGS += -DGIT_REV=$$system(type .git\refs\heads\master)
message(QMAKE_CXXFLAGS=$$QMAKE_CXXFLAGS)
