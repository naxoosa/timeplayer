TEMPLATE = app

QT += qml quick widgets sql network core concurrent
CONFIG += c++11

SOURCES += main.cpp \
    applicationsignal.cpp \
    messageobject.cpp \
    window.cpp \
    items/CustomProgressBar.cpp \
    pause.cpp \
    report.cpp \
    absence.cpp \
    absrepository.cpp \
    configuration.cpp \
    employee.cpp \
    employeerepository.cpp \
    pauserepository.cpp \
    reportrepository.cpp \
    singleapplication.cpp \
    user.cpp \
    thread.cpp \
    detaildialog.cpp

RESOURCES += qml.qrc
RC_ICONS = contents/naxplayerico.ico

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

DISTFILES +=     

HEADERS += \
    applicationsignal.h \
    messageobject.h \
    window.h \
    items/CustomProgressBar.h \
    pause.h \
    report.h \
    absence.h \
    absrepository.h \
    configuration.h \
    employee.h \
    employeerepository.h \
    pauserepository.h \
    reportrepository.h \
    singleapplication.h \
    singleton.h \
    user.h \
    thread.h \
    detaildialog.h

win32:contains(QMAKE_HOST.arch,x86_64):{
    LIBS+="C:\Program Files\MySQL\MySQL Server 5.7\lib\libmysql.lib"
    resources.files += libs64\libmysql.dll
    CONFIG(debug, debug|release) {
        DESTDIR = debug64
    } else {
        DESTDIR = release64
    }
}
else {
    LIBS+="C:\Program Files (x86)\MySQL\MySQL Server 5.7 32bits\lib\libmysql.lib"
    resources.files += libs32\libmysql.dll
    CONFIG(debug, debug|release) {
        DESTDIR = debug32
    } else {
        DESTDIR = release32
    }
}

#DESTDIR = debug

resources.path = $$DESTDIR
resources.files += *.ini *.template

INSTALLS += resources

FORMS += \
    detaildialog.ui

