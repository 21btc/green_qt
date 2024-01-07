import Blockstream.Green
import Blockstream.Green.Core
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import QtMultimedia

import "analytics.js" as AnalyticsJS
import "util.js" as UtilJS

ApplicationWindow {
    Constants {
        id: constants
    }
    id: window
    x: Settings.windowX
    y: Settings.windowY
    width: Settings.windowWidth
    height: Settings.windowHeight
    onXChanged: Settings.windowX = x
    onYChanged: Settings.windowY = y
    onWidthChanged: Settings.windowWidth = width
    onHeightChanged: Settings.windowHeight = height
    minimumWidth: 1024
    minimumHeight: 768
    visible: true
    color: '#121416'
    title: {
        const title = stack_view.currentItem?.title
        const parts = []
        if (title) parts.push(title)
        parts.push('Blockstream Green');
        if (env !== 'Production') parts.push(`[${env}]`)
        return parts.join(' - ');
    }

    Label {
        parent: Overlay.overlay
        visible: false
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 4
        z: 1000
        HoverHandler {
            id: debug_focus_hover_handler
        }
        opacity: debug_focus_hover_handler.hovered ? 1.0 : 0.3
        background: Rectangle {
            color: Qt.rgba(1, 0, 0, 0.8)
            radius: 4
            border.width: 2
            border.color: 'black'
        }
        padding: 8
        text: {
            const parts = []
            let item = activeFocusItem
            while (item) {
                parts.unshift((''+item).split('(')[0])
                item = item.parent
            }
            return parts.join(' > ')
        }
    }

    GStackView {
        id: stack_view
        anchors.fill: parent
        initialItem: splash_page
    }

    Component {
        id: splash_page
        SplashPage {
            onTimeout: app_page.active = true
        }
    }

    Loader {
        id: app_page
        active: false
        visible: false
        asynchronous: true
        onLoaded: stack_view.replace(null, app_page.item, StackView.PushTransition)
        sourceComponent: AppPage {
        }
    }

    AnalyticsConsentDialog {
        property real offset_y
        id: consent_dialog
        x: parent.width - consent_dialog.width - constants.s2
        y: parent.height - consent_dialog.height - constants.s2 - 30 + consent_dialog.offset_y
        // by default dialogs height depends on y, break that dependency to avoid binding loop on y
        height: implicitHeight
        visible: Settings.analytics === ''
        enter: Transition {
            SequentialAnimation {
                PropertyAction { property: 'x'; value: 0 }
                PropertyAction { property: 'offset_y'; value: 100 }
                PropertyAction { property: 'opacity'; value: 0 }
                PauseAnimation { duration: 2000 }
                ParallelAnimation {
                    NumberAnimation { property: 'opacity'; to: 1; easing.type: Easing.OutCubic; duration: 1000 }
                    NumberAnimation { property: 'offset_y'; to: 0; easing.type: Easing.OutCubic; duration: 1000 }
                }
            }
        }
    }

    MediaDevices {
        id: media_devices
    }
    readonly property bool hasVideoInput: media_devices.videoInputs?.length > 0
}
