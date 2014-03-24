//.pragma library

//function calculateShowOrder(flag_array, index_of_cell) {
//    var flag = 0;
////    console.log("In calculateShowOrder function, flag_array: ",
////                flag_array);
//    for (var i = 0; i < month_view.max_show_events_of_day; ++i) {
//        if (flag_array[index_of_cell][i] === 0) {
//            flag = i;
//            break;
//        }
//    }
//    return flag;
//}

function getEditViewPosX(mouse_x) {
    var show_pos_x = mouse_x;
    var view_width = float_edit.width
    var root_width = month_view.width

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
    var view_height = float_edit.height;
    var cell_cols = month_view.columns;

    if (index_of_cell < cell_cols) {
        show_pos_y = mouse_y;
    } else {
        show_pos_y = mouse_y - view_height;
    }

    return show_pos_y;
}
