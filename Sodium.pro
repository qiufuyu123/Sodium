QT       += core gui
QT       +=  multimedia
RESOURCES += qdarkstyle/dark/darkstyle.qrc \
    ico.qrc
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
include(qhotkey/qhotkey.pri)
CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    audioprogress.cpp \
    keydialog.cpp \
    main.cpp \
    mainform.cpp \
    settings.cpp

HEADERS += \
    audioprogress.h \
    keydialog.h \
    mainform.h \
    settings.h

FORMS += \
    keydialog.ui \
    mainform.ui
RC_ICONS = ico.ico
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
