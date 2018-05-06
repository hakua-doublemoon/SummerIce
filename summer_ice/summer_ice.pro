#TEMPLATE = subdirs

#SUBDIRS += summer_clock.pro ice_clock.pro
#SUBDIRS += summer_clock.pro

QT += qml quick
#QT += multimedia

android{
    #QT -= multimedia
    QT += androidextras
    QT += remoteobjects

    REPC_REPLICA += pingpong.rep
    REPC_SOURCE  += pingpong.rep
}

CONFIG += c++11

SOURCES += summer_clock.cpp \
    execute/fakestar.cpp \
    execute/ice.cpp \
    #execute/sky.cpp \
    execute/snow.cpp \
    execute/star.cpp

HEADERS += \
    execute/fakestar.h \
    execute/ice.h \
    #execute/sky.h \
    execute/snow.h \
    execute/star.h

android{
SOURCES += execute/sky.cpp
HEADERS += execute/sky.h
}

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

android{
    DISTFILES += android/AndroidManifest.xml \
        android/src/com/summer/test/Spring.java \
        android/src/com/summer/test/AutumnBCasRcv.java \
        android/src/com/summer/test/Winter.java

    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
}

