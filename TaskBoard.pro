QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

win32:VERSION = 0.1.1.0 # major.minor.patch.build
else:VERSION = 0.1.1    # major.minor.patch

DEFINES += APP_VERSION=\"\\\"$${VERSION}\\\"\"
DEFINES += APP_NAME=\"\\\"TaskBoard\\\"\"

win32 {
    message("Build for Windows")
    QMAKE_CXXFLAGS_RELEASE -= -O
    QMAKE_CXXFLAGS_RELEASE -= -O1
    QMAKE_CXXFLAGS_RELEASE *= -O2
    DEFINES += APP_OS=\"\\\"Windows\\\"\"
    DEFINES += APP_OS_VERSION=\"\\\"$$system(ver)\\\"\"
    equals(QMAKE_TARGET.arch, arm64) {
        message("CPU Architecture : aarch64")
        DEFINES += APP_ARCH=\"\\\"arm64\\\"\"
    }
    equals(QMAKE_TARGET.arch, x86_64) {
        message("CPU Architecture : x64")
        QMAKE_CXXFLAGS_RELEASE += -favor:INTEL64
        DEFINES += APP_ARCH=\"\\\"x64\\\"\"
    }
    RC_ICONS = icon.ico
    QMAKE_TARGET_COMPANY = "Aurelie Delhaie"
    QMAKE_TARGET_PRODUCT = "TaskBoard"
    QMAKE_TARGET_DESCRIPTION = "TaskBoard"
}

macx {
    message("Build for macOS")
    ICON = icon.icns
    QMAKE_INFO_PLIST = Info.plist
    QMAKE_CXXFLAGS_RELEASE -= -O
    QMAKE_CXXFLAGS_RELEASE -= -O1
    QMAKE_CXXFLAGS_RELEASE -= -O2
    QMAKE_CXXFLAGS_RELEASE *= -O3
    QMAKE_APPLE_DEVICE_ARCHS = x86_64 arm64
    DEFINES += APP_OS=\"\\\"macOS\\\"\"
    DEFINES += APP_OS_VERSION=\"\\\"$$system(uname -r)\\\"\"
    DEFINES += APP_ARCH=\"\\\"universal\\\"\"
}

linux-* {
    message("Build for Linux")
    DEFINES += APP_OS=\"\\\"$$system(cat /etc/issue | cut -d\' \' -f1)\\\"\"
    DEFINES += APP_OS_VERSION=\"\\\"$$system(uname -r)\\\"\"
    DEFINES += APP_ARCH=\"\\\"$$system(uname -m)\\\"\"
    ARCH = $$system(uname -m)
    equals(ARCH, aarch64) {
        message("CPU Architecture : aarch64")
        QMAKE_CXXFLAGS_RELEASE += -mtune=cortex-a72
    }
    equals(ARCH, amd64) {
        message("CPU Architecture : amd64")
        QMAKE_CXXFLAGS_RELEASE += -march=skylake
    }
    QMAKE_CXXFLAGS_RELEASE *= -O3
}

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/frames/aboutdialog.cpp \
    src/models/board.cpp \
    src/main.cpp \
    src/frames/mainwindow.cpp \
    src/frames/namedialog.cpp \
    src/frames/prefdialog.cpp \
    src/models/priority.cpp \
    src/models/status.cpp \
    src/models/task.cpp \
    src/frames/taskdialog.cpp \
    src/tools.cpp

HEADERS += \
    src/frames/aboutdialog.h \
    src/models/board.h \
    src/frames/mainwindow.h \
    src/frames/namedialog.h \
    src/frames/prefdialog.h \
    src/models/priority.h \
    src/models/status.h \
    src/models/task.h \
    src/frames/taskdialog.h \
    src/tools.h

FORMS += \
    src/frames/aboutdialog.ui \
    src/frames/mainwindow.ui \
    src/frames/namedialog.ui \
    src/frames/prefdialog.ui \
    src/frames/taskdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
