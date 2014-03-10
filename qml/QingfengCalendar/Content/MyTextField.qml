import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

Item {
    id: textfield

    implicitWidth: 100
    implicitHeight: 20

    activeFocusOnTab: true

    signal accepted()

    property alias contentHeight: text_input.contentHeight
    property alias contentWidth: text_input.contentWidth

//    Accessible.name: text
//    Accessible.role: Accessible.

    MouseArea {
        id: mouse_area
        anchors.fill: parent
        hoverEnabled: true
        onEntered: cursorShape = Qt.IBeamCursor
        onExited: cursorShape = Qt.ArrowCursor
        onClicked: textfield.forceActiveFocus()
    }

    SystemPalette {
        id: __syspal
        colorGroup: textfield.enabled ? SystemPalette.Active :
                                      SystemPalette.Disabled
    }

    property font font

    property color textColor: __syspal.text

    property color selectionColor: __syspal.highlight

    property color selectedTextColor: __syspal.highlightedText

    Item {
        implicitWidth: Math.round(contentHeight * 8)
        implicitHeight: Math.max(25, Math.round(contentHeight * 1.2))

        anchors.fill: parent

        Rectangle {
            anchors.fill: parent
            anchors.bottomMargin: -1
            color: "#44ffffff"
            radius: baserect.radius
        }
        Rectangle {
            id: baserect
            gradient: Gradient {
                GradientStop {color: "#e0e0e0" ; position: 0}
                GradientStop {color: "#fff" ; position: 0.1}
                GradientStop {color: "#fff" ; position: 1}
            }
            radius: contentHeight * 0.16
            anchors.fill: parent
            border.color: textfield.activeFocus ? "#47b" : "#999"
        }
    }

    TextInput {
        id: text_input
        focus: true
        selectByMouse: true
        selectionColor: selectionColor
        selectedTextColor: selectedTextColor

        font: font
//        font: __panel ? __panel.
//        anchors.leftMargin:  0
//        anchors.topMargin:  0
//        anchors.rightMargin:  0
//        anchors.bottomMargin: 0

        anchors.fill: parent
        verticalAlignment: Text.AlignVCenter

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
