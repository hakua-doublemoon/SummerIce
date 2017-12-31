QT += qml quick
#QT += multimedia

android{
    #QT -= multimedia
    QT += androidextras
    QT += remoteobjects

    REPC_REPLICA += pingpong.rep
    #REPC_SOURCE += pingpong.rep
}

CONFIG += c++11

SOURCES += summer_clock.cpp \
    #main.cpp \
    ice/ice.cpp \
    sky/sky.cpp \
    star/fakestar.cpp

android{
    SOURCES += star/star.cpp
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

HEADERS += \
    ice/ice.h \
    sky/sky.h \
    star/fakestar.h
android{
    HEADERS += star/star.h
}

android{
    DISTFILES += \
    #   MyService.java
        android/AndroidManifest.xml \
        android/src/com/summer/test/Spring.java
    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
}

DISTFILES += \
    android/src/com/summer/test/Winter.java
