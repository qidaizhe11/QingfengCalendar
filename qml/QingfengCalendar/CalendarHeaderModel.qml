import QtQuick 2.0

ListModel {
    id: root

    property var locale

    Component.onCompleted: {
        var daysOfWeek = [Locale.Sunday, Locale.Monday, Locale.Tuesday,
                Locale.Wednesday, Locale.Thursday, Locale.Friday,
                Locale.Saturday];
        var firstDayOfWeek = root.locale.firstDayOfWeek;

        var shifted = daysOfWeek.splice(firstDayOfWeek,
                                        daysOfWeek.length - firstDayOfWeek);
        daysOfWeek = shifted.concat(daysOfWeek);

        for (var i = 0; i < daysOfWeek.length; ++i) {
            var element = { dayOfWeek: daysOfWeek[i] }
            root.append(element);
        }
    }
}
