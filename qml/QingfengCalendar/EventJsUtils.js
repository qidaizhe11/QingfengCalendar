.pragma library

function initEventCountArray(array, number) {
    for( var i = 0; i < number; ++i) {
        array.push(0);
    }
}

function initShowFlagsArray(array, max_gird_cells, max_show_events_of_day) {
    for( var i = 0; i < max_gird_cells; ++i) {
        array[i] = [];
        for (var flag = 0; flag < max_show_events_of_day; ++flag) {
            array[i][flag] = 0;
        }
    }
}

function calculateShowFlag(flag_array, index_of_cell, max_show_events_of_day) {
    var flag = 0;
    for (var i = 0; i < max_show_events_of_day; ++i) {
        if (flag_array[index_of_cell][i] === 0) {
            flag = i;
            break;
        }
    }
    return flag;
}

function increaseEventCount(count_array, begin, last_days, max_size) {
    for (var i = 0; i < last_days; ++i) {
        if (begin + i < max_size) {
            count_array[begin + i] += 1;
        }
    }
}

function increaseShowFlag(show_flag_array, flag, start_day, last_days, max_grid_cells) {
    for (var i = 0; i < last_days; ++i) {
        if (start_day + last_days < max_grid_cells) {
            show_flag_array[start_day + i][flag] = 1;
        }
    }
}

function getEditViewPosY(mouse_y, view_height, index_of_cell, cell_cols) {
    var show_pos_y = mouse_y;

    if (index_of_cell < cell_cols) {
        show_pos_y = mouse_y;
    } else {
        show_pos_y = mouse_y - view_height;
    }

    return show_pos_y;
}

function getEditViewPosX(mouse_x, view_width, root_width) {
    var show_pos_x = mouse_x;

    if (mouse_x - view_width / 2 < 0) {
        show_pos_x = 0;
    } else if (mouse_x + view_width / 2 > root_width) {
        show_pos_x = root_width - view_width;
    } else {
        show_pos_x = mouse_x - view_width / 2;
    }

    return show_pos_x;
}
