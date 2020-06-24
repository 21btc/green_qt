TARGET = Green

VERSION_MAJOR = 0
VERSION_MINOR = 0
VERSION_PATCH = 3
VERSION_PRERELEASE =
VERSION = $${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_PATCH}

QMAKE_TARGET_COMPANY = Blockstream Corporation Inc.
QMAKE_TARGET_PRODUCT = Blockstream Green
QMAKE_TARGET_DESCRIPTION = Blockstream Green
QMAKE_TARGET_COPYRIGHT = Copyright 2020 Blockstream Corporation Inc. All rights reserved.

DEFINES += "VERSION_MAJOR=$$VERSION_MAJOR"\
       "VERSION_MINOR=$$VERSION_MINOR"\
       "VERSION_PATCH=$$VERSION_PATCH" \
       "VERSION_PRERELEASE=\"$$VERSION_PRERELEASE\"" \
       "VERSION=\"$${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_PATCH}\""

QT += qml quick quickcontrols2 svg

CONFIG += c++11 qtquickcompiler

CONFIG += qzxing_qml qzxing_multimedia enable_decoder_qr_code enable_encoder_qr_code

include($$(BUILDROOT)/qzxing/src/QZXing-components.pri)

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS QZXING_QML

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/accountcontroller.cpp \
    src/account.cpp \
    src/asset.cpp \
    src/balance.cpp \
    src/clipboard.cpp \
    src/controller.cpp \
    src/createaccountcontroller.cpp \
    src/ga.cpp \
    src/json.cpp \
    src/main.cpp \
    src/network.cpp \
    src/renameaccountcontroller.cpp \
    src/sendtransactioncontroller.cpp \
    src/settingscontroller.cpp \
    src/transaction.cpp \
    src/twofactorcontroller.cpp \
    src/util.cpp \
    src/wallet.cpp \
    src/walletmanager.cpp \
    src/wally.cpp

HEADERS += \
    src/accountcontroller.h \
    src/account.h \
    src/asset.h \
    src/balance.h \
    src/clipboard.h \
    src/controller.h \
    src/createaccountcontroller.h \
    src/ga.h \
    src/json.h \
    src/network.h \
    src/renameaccountcontroller.h \
    src/sendtransactioncontroller.h \
    src/settingscontroller.h \
    src/transaction.h \
    src/twofactorcontroller.h \
    src/util.h \
    src/wallet.h \
    src/walletmanager.h \
    src/wally.h

RESOURCES += qml/qml.qrc
RESOURCES += assets/assets.qrc
win32 {
    RESOURCES += src/win.qrc
} else {
    RESOURCES += src/linux.qrc
}

CONFIG += lrelease embed_translations

EXTRA_TRANSLATIONS = $$files($$PWD/i18n/*.ts)

GDK_BUILD_DIR = $$absolute_path($$(GDK_PATH), $${PWD})
INCLUDEPATH += $${GDK_BUILD_DIR}

macos {
    QMAKE_TARGET_BUNDLE_PREFIX = com.blockstream
    LIBS += -framework Foundation -framework Cocoa
}

macos {
    ICON = Green.icns

    QMAKE_POST_LINK += \
        plutil -replace CFBundleDisplayName -string \"Blockstream Green\" $$OUT_PWD/$${TARGET}.app/Contents/Info.plist && \
        plutil -replace NSCameraUsageDescription -string \"We use the camera to scan QR codes\" $$OUT_PWD/$${TARGET}.app/Contents/Info.plist && \
        plutil -remove NOTE $$OUT_PWD/$${TARGET}.app/Contents/Info.plist || true

    static {
        LIBS += $$GDK_BUILD_DIR/libgreenaddress_full.a
    } else {
        LIBS += -L$$GDK_BUILD_DIR/build-clang/src/ -lgreenaddress
    }
}

unix:!macos:!android {
    static {
        LIBS += $$GDK_BUILD_DIR/libgreenaddress_full.a
        SOURCES += src/glibc_compat.cpp
        LIBS += -Wl,--wrap=__divmoddi4 -Wl,--wrap=log2f
    } else {
        LIBS += -L$$GDK_BUILD_DIR/build-gcc/src -lgreenaddress
    }
}

win32:static {
    # FIXME: the following script appends -lwinpthread at the end so that green .rsrc entries are used instead
    QMAKE_LINK=$${PWD}/link.sh
    RC_ICONS = Green.ico
    LIBS += $$GDK_BUILD_DIR/libgreenaddress_full.a
}

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    src/qtquickcontrols2.conf
