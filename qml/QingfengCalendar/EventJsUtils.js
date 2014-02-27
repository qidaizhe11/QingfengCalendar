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
