
var EventsInfo = {
    createNew: function(max_shown_events) {
        var events_info = {};

        events_info.count = 0;
        events_info.show_order = [];
        for (var i = 0; i < max_shown_events; ++i) {
            events_info.show_order.push(0);
        }

        events_info.increaseInfo = function(order_index) {
            ++events_info.count;
            ++events_info.show_order[order_index];
        }

        return events_info;
    }
}
