import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

Button {
    property color button_color
//    property color text_color: "black"
    property real font_size: 12
    property bool font_bold: false

    style: ButtonStyle {

        background:  Rectangle {
            anchors.fill: parent

            border.color: control.activeFocus ? "#47b" :
                                                (button_color ? button_color : "#999")
            implicitWidth: 100
            implicitHeight: 20
            gradient: Gradient {
                GradientStop {color: control.pressed ? "#aaa" : "#fefefe" ;
                    position: 0}
                GradientStop {color: control.pressed ? "#ccc" : "#e3e3e3";
                    position: control.pressed ? 0.1: 1}
            }
//            gradient: Gradient {
//                GradientStop { color: "#fefefe"; position: 0}
//                GradientStop { color: "#e3e3e3"; position: 1}
//            }

            radius: height * 0.08

            Rectangle {
                anchors.fill: parent
                radius: parent.radius
                color: control.hovered || control.activeFocus ? "#47b" : "white"
                opacity: control.hovered || control.activeFocus ? 0.1 : 0
                Behavior on opacity {NumberAnimation{ duration: 100 }}
            }
        }

        label: Item {
            Text {
                id: text
                anchors.centerIn: parent

                renderType: Text.NativeRendering
                anchors.verticalCenter: parent.verticalCenter
                text: control.text
                font.pointSize: font_size
                font.bold: font_bold
                color: button_color ? button_color : __syspal.text
            }
        }
    }
}
