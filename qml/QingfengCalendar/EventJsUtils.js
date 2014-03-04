//.pragma library

function initEventCountArray(array) {
    for( var i = 0; i < panelItem.total_cells; ++i) {
        array.push(0);
    }
}

function initShowFlagsArray(array) {
    for( var i = 0; i < panelItem.total_cells; ++i) {
        array[i] = [];
        for (var flag = 0; flag < panelItem.max_show_events_of_day; ++flag) {
            array[i][flag] = 0;
        }
    }
}

function calculateShowFlag(flag_array, index_of_cell) {
    var flag = 0;
//    console.log("In calculateShowFlag function, flag_array: ",
//                flag_array);
    for (var i = 0; i < panelItem.max_show_events_of_day; ++i) {
        if (flag_array[index_of_cell][i] === 0) {
            flag = i;
            break;
        }
    }
    return flag;
}

function increaseEventCount(count_array, begin, last_days) {
    var max_size = panelItem.total_cells;
    for (var i = 0; i < last_days; ++i) {
        if (begin + i < max_size) {
            count_array[begin + i] += 1;
        }
    }
}

function increaseShowFlag(show_flag_array, flag, start_day, last_days) {
    var max_grid_cells = panelItem.total_cells;
    for (var i = 0; i < last_days; ++i) {
        if (start_day + last_days < max_grid_cells) {
            show_flag_array[start_day + i][flag] = 1;
        }
    }
}

function getEditViewPosX(mouse_x) {
    var show_pos_x = mouse_x;
    var view_width = float_event_edit.width
    var root_width = panelItem.width

    if (mouse_x - view_width / 2 < 0) {
        show_pos_x = 0;
    } else if (mouse_x + view_width / 2 > root_width) {
        show_pos_x = root_width - view_width;
    } else {
        show_pos_x = mouse_x - view_width / 2;
    }

    return show_pos_x;
}

function getEditViewPosY(mouse_y, index_of_cell) {
    var show_pos_y = mouse_y;
    var view_height = float_event_edit.height;
    var cell_cols = panelItem.columns;

    if (index_of_cell < cell_cols) {
        show_pos_y = mouse_y;
    } else {
        show_pos_y = mouse_y - view_height;
    }

    return show_pos_y;
}
