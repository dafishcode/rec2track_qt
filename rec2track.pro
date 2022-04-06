#-------------------------------------------------
#
# Project created by QtCreator 2019-07-04T16:32:35
#
#-------------------------------------------------
## After installing OpenCv 3.6, with opencv_contrib and QT_ON - verify with pkg-config --modversion opencv
## After install QT 5.15.1 I needed to qtchooser -install qt5.15.1 to : qtchooser -install qt5.15.1 /opt/Qt/5.15.1/gcc_64/bin/qmake
QT       += core gui
QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DESTDIR = bin
OBJECTS_DIR = .obj

TARGET = rec2track
TEMPLATE = app

SOURCES +=\
    src/camera_functions.cpp \
           src/barrage.cpp\
    src/livetracking_handlers.cpp \
           src/mainwindow.cpp \
    src/main.cxx

HEADERS  +=\
            include/camera_functions.h \
            include/circular_video_buffer_ts.h \
            include/Point.h\
            include/barrage.h\
            include/livetracking_handlers.h \
            include/mainwindow.h

FORMS    += form/mainwindow.ui
#-I /usr/local/include/opencv
##
##INCLUDEPATH += -I /usr/include/flycapture  -I/usr/share/qt5/mkspecs/linux-g++-64
##INCLUDEPATH += `pkg-config --cflags opencv `
####INCLUDEPATH += -I/opt/Qt/5.15.2/gcc_64/include/
####INCLUDEPATH += -I/opt/Qt/5.11.2/gcc_64/include/


##LIBS += `pkg-config --libs opencv ` -L/usr/local/lib/ -lflycapture  -lboost_thread -lboost_system -lgsl -lblas -L/usr/lib/x86_64-linux-gnu
#LIBS += -L/opt/Qt/5.15.2/gcc_64/lib
#LIBS += -L/opt/Qt/5.11.2/gcc_64/lib


INCLUDEPATH += -I /usr/include/flycapture -I /usr/local/include/opencv -I/usr/share/qt4/mkspecs/linux-g++-64
LIBS += `pkg-config --libs opencv` -lflycapture -L/usr/lib/x86_64-linux-gnu -lboost_thread -lboost_system -lgsl -lblas
