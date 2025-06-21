QT       += core gui
QT += multimedia multimediawidgets
QT += serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    brushlessmodule.cpp \
    camera_dialog.cpp \
    camera_widget.cpp \
    comminterface.cpp \
    distance_dialog.cpp \
    input_dialog.cpp \
    main.cpp \
    mainwindow.cpp \
    myserialport.cpp \
    parameters_dialog.cpp \
    program_dialog.cpp \
    serialcom.cpp

HEADERS += \
    BrushlessModuleDefs.h \
    brushlessmodule.h \
    camera_dialog.h \
    camera_widget.h \
    comminterface.h \
    distance_dialog.h \
    input_dialog.h \
    mainwindow.h \
    myserialport.h \
    parameters_dialog.h \
    program_dialog.h \
    serialcom.h

FORMS += \
    camera_dialog.ui \
    camera_widget.ui \
    distance_dialog.ui \
    input_dialog.ui \
    mainwindow.ui \
    parameters_dialog.ui \
    program_dialog.ui

TRANSLATIONS += \
    SR2700_en_150.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += images.qrc
