import QtQuick 2.0

ListModel {
    id: root

//    property var locale

    Component.onCompleted: {
        var daysOfWeek = [Locale.Sunday, Locale.Monday, Locale.Tuesday,
                Locale.Wednesday, Locale.Thursday, Locale.Friday,
                Locale.Saturday];
//        var daysOfWeek = [Locale.Monday, Locale.Tuesday,
//                Locale.Wednesday, Locale.Thursday, Locale.Friday,
//                Locale.Saturday, Locale.Sunday];
//        var firstDayOfWeek = Qt.locale().firstDayOfWeek;
        var firstDayOfWeek = 1;

        var shifted = daysOfWeek.splice(firstDayOfWeek,
                                        daysOfWeek.length - firstDayOfWeek);
        daysOfWeek = shifted.concat(daysOfWeek);

        for (var i = 0; i < daysOfWeek.length; ++i) {
            var element = { dayOfWeek: daysOfWeek[i] }
            root.append(element);
        }
    }
}
