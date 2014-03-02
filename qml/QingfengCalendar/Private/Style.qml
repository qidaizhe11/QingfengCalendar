import QtQuick 2.0
import QtQuick.Controls 1.1
import MyCalendar.Controls.Private 1.0

AbstractStyle {
    readonly property Item control: __control

    property var __syspal: SystemPalette {
        colorGroup: control.enabled ? SystemPalette.Active :
                                      SystemPalette.Disabled
    }
}
