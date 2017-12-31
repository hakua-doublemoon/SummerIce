TEMPLATE = lib
TARGET = ice_clock
CONFIG += dll
QT += core
QT += qml
#QT += quick

#QT += QTextIStream QTextOStream

android{
    QT += androidextras
    QT += remoteobjects
}

REPC_SOURCE += pingpong.rep
#REPC_REPLICA += pingpong.rep

SOURCES += ice_clock.cpp

android{
    SOURCES += snow/snow.cpp \
               ice/ice.cpp
    HEADERS += snow/snow.h \
               ice/ice.h
}

android{
    DISTFILES += \
        android/src/com/summer/test/MyBroadcastReceiver.java \
        android/src/com/summer/test/Autumn.java \
        android/src/com/summer/test/AutumnBCasRcv.java
}
