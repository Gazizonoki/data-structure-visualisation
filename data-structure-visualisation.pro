INCLUDEPATH += "include/"
include ( ./qwt/qwt.prf )

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += qwt
CONFIG += exceptions_off

HEADERS += \
    include/app.h \
    include/avl_tree.h \
    include/controller.h \
    include/mainwindow.h \
    include/observer/observer.h \
    include/observer/observable.h \
    include/treewindow.h

SOURCES += \
    main.cpp \
    src/app.cpp \
    src/model/avl_tree.cpp \
    src/view/mainwindow.cpp \
    src/controller/controller.cpp \
    src/view/treewindow.cpp

FORMS += src/view/mainwindow.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
