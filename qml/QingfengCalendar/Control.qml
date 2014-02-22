import QtQuick 2.1
import QtQuick.Controls.Styles 1.1

FocusScope {
    id: root

    property Component style

    property QtObject __style: styleLoader.item

    property Item __panel: panelLoader.item

    property var styleHints

    implicitHeight: __panel ? __panel.implicitHeight : 0
    implicitWidth: __panel ? __panel.implicitWidth : 0
    baselineOffset: __panel ? __panel.baselineOffset : 0
    activeFocusOnTab: false

    property alias __styleData: styleLoader.styleData

    Loader {
        id: panelLoader
        anchors.fill: parent
        sourceComponent: __style ? __style.panel : null
        onStatusChanged: {
            if (status == Loader.Error)
                console.error("Failed to load Style for", root)
        }
        Loader {
            id: styleLoader
            sourceComponent: style
            property Item __control: root
            property QtObject styleData: null
            onStatusChanged: {
                if (status == Loader.Error)
                    console.error("Failed to load Style for", root)
            }
        }
    }
}
