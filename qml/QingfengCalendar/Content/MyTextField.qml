import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

FocusScope {
    id: textfield

    signal accepted()

    property alias text: text_input.text

    activeFocusOnTab: true

    implicitWidth: background.implicitWidth ? background.implicitWidth : 100
    implicitHeight: background.implicitHeight ? background.implicitHeight : 20

    property alias contentHeight: text_input.contentHeight
    property alias contentWidth: text_input.contentWidth

    SystemPalette {
        id: __syspal
        colorGroup: textfield.enabled ? SystemPalette.Active :
                                      SystemPalette.Disabled
    }

    property font font
    property alias font_size: text_input.font.pointSize

    property alias border_radius: background.radius
//    property alias border_width: bac

    property color textColor: __syspal.text

    property color selectionColor: __syspal.highlight

    property color selectedTextColor: __syspal.highlightedText

    Rectangle {
        id: background

        implicitWidth: Math.round(contentHeight * 8)
        implicitHeight: Math.max(30, Math.round(contentHeight * 1.2))

        anchors.fill: parent

        gradient: Gradient {
            GradientStop {color: "#e0e0e0" ; position: 0}
            GradientStop {color: "#fff" ; position: 0.1}
            GradientStop {color: "#fff" ; position: 1}
        }
        radius: contentHeight * 0.16

        border.color: textfield.activeFocus ? "#47b" : "#999"
    }

    MouseArea {
        id: mouse_area

        anchors.fill: parent
        hoverEnabled: true
        onEntered: cursorShape = Qt.IBeamCursor
        onExited: cursorShape = Qt.ArrowCursor
        onClicked: textfield.forceActiveFocus()
    }

    TextInput {
        id: text_input
        focus: true
        selectByMouse: true
        selectionColor: selectionColor
        selectedTextColor: selectedTextColor

        font: font
        anchors.leftMargin:  4
        anchors.topMargin:  4
        anchors.rightMargin:  4
        anchors.bottomMargin: 4

        anchors.fill: parent
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: TextInput.AlignLeft

        color: textColor
        clip: contentWidth > width

        renderType: Text.NativeRendering

        Keys.forwardTo: textfield

        onAccepted: {
            Qt.inputMethod.commit()
            Qt.inputMethod.hide()
            textfield.accepted()
        }

        //        onEditingFinished: textfield.editingFinished()
    }
}
