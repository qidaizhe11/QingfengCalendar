import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

ComboBox {

    style: ComboBoxStyle {
        background: Item {
            anchors.fill: parent
            implicitWidth: 100
            implicitHeight: 25
            Rectangle {
                anchors.fill: parent

                border.color: control.activeFocus ? "#47b" : "#999"
                gradient: Gradient {
                    GradientStop {color: control.pressed ? "#aaa" : "#fefefe" ;
                        position: 0}
                    GradientStop {color: control.pressed ? "#ccc" : "#e3e3e3";
                        position: control.pressed ? 0.1: 1}
                }

                radius: height * 0.08

                Rectangle {
                    anchors.fill: parent
                    radius: parent.radius
                    color: control.hovered || control.activeFocus ? "#47b" : "white"
                    opacity: control.hovered || control.activeFocus ? 0.1 : 0
                    Behavior on opacity {NumberAnimation{ duration: 100 }}
                }
            }

            Image {
                id: imageItem
                source: "../images/arrow-7-down.png"
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 2
                opacity: control.enabled ? 0.6 : 0.5
            }
        }
    }
}
