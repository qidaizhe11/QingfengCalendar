import QtQuick 2.1

Rectangle {
    id: root
    width: 600
    height: 500

    Repeater {
        model: 3
        delegate: delegateItem
    }

    Component {
        id: delegateItem

        Item {
            id: cell
            width: 100
            height: 100
            x: Math.random() * root.width
            y: Math.random() * root.height

            Rectangle {
                id: handle
                width: parent.width
                height: 20
                color: handleMouseArea.pressed ? "orange" : "red"

                MouseArea {
                    id: handleMouseArea
                    anchors.fill: parent
                    drag.target: handle
                    drag.axis: Drag.YAxis
                }
            }

            Rectangle {
                id: content
                property bool handleOnTop: true
                property bool lastConfig: true
                anchors.top: handle.bottom
                anchors.bottom: cell.bottom
                width: parent.width
                color: contentMouseArea.pressed ? "orange" : "blue"

                MouseArea {
                    id: contentMouseArea
                    anchors.fill: parent
                    drag.target: cell
                }

                onHandleOnTopChanged: {
                    if (handleOnTop) {
                        anchors.top = handle.bottom;
                        anchors.bottom = cell.bottom;
                    } else {
                        anchors.bottom = handle.top;
                        anchors.top = cell.bottom;
                    }
                }

                onHeightChanged: {
                    // the lastConfig flag is a workaround to avoid the slot
                    // onHandleOnTopChanged tobe executed more than one time
                    // when the mouse is moved too fast during the dragging.
                    if (height <= -handle.height && lastConfig === handleOnTop) {
                        handleOnTop = !handleOnTop;
                        lastConfig = handleOnTop;
                    }
                }
            }
        }
    }
}
