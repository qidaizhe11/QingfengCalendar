
.pragma library

var daysInAWeek = 7;
var monthsInYear = 12;

var weeksOnCalendarMonth = 6;

var minimumCalendarDate = new Date(-1, 0, 1);
minimumCalendarDate.setFullYear(minimumCalendarDate.getFullYear() + 2);
var maximumCalendarDate = new Date(275759, 9, 25);

function daysInMonth(date){
    // Passing 0 as the day will give us the previous month, which will be
    // date.getMonth() since we added 1 to it.
    return new Date(date.getFullYear(), date.getMonth() + 1, 0).getDate();
}

//
// Returns a copy of @date with its month set to @month, keeping the same
// day if possible. Does not modify @date.
//
function setMonth(date, month) {
    var oldDay = date.getDate();
    var newDate = new Date(date);
    //Set the day first, because setting the month could cause it to skip ahead
    // a month if the day is larger than the lastest day in that month.
    newDate.setDate(1);
    newDate.setMonth(month);
    // We'd like to have the previous day still selected when we change months,
    // but it might not be possible, so use the smallest of the two.
    newDate.setDate(Math.min(oldDay, daysInMonth(newDate)));
    return newDate;
}

function cellRectAt(index, columns, rows, availableWidth, availableHeight) {
    var col = Math.floor(index % columns);
    var row = Math.floor(index / columns);

    var remainingHorizontalSpace = Math.floor(availableWidth % columns);
    var remainingVerticalSpace = Math.floor(availableHeight % rows);
    var baseCellWidth = Math.floor(availableWidth / columns);
    var baseCellHeight = Math.floor(availableHeight / rows);

    var rect = Qt.rect(0, 0, 0, 0);

    rect.x = baseCellWidth * col;
    rect.width = baseCellWidth;
    if (remainingHorizontalSpace > 0) {
        if (col < remainingHorizontalSpace) {
            ++rect.width;
        }

        // This cell's x position should be increased by 1 for every column
        // above it.
        rect.x += Math.min(remainingHorizontalSpace, col);
    }

    rect.y = baseCellHeight * row;
    rect.height = baseCellHeight;
    if (remainingVerticalSpace > 0) {
        if (row < remainingVerticalSpace) {
            ++rect.height;
        }

        // This cell's y position should be increased by 1 for every row
        // above it.
        rect.y += Math.min(remainingVerticalSpace, row);
    }

    return rect;
}
