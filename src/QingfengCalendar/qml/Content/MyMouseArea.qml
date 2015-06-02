import QtQuick 2.1

MouseArea {
    hoverEnabled: true
    onEntered: cursorShape = Qt.OpenHandCursor
    onExited: cursorShape = Qt.ArrowCursor
}
