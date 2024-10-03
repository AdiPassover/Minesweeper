#include "cell.h"

char cell_to_char(Cell cell) { // TODO missing mine exploded
    if (!cell.is_revealed) {
        if (!cell.is_flagged) { // hidden empty cell
            return HIDDEN_CHAR;
        } else { // flagged cell
            return FLAGGED_CHAR;
        }
    } else { // revealed
        if (cell.type == MINE) {
            if (cell.is_flagged)
                return FLAGGED_CHAR;
            return MINE_CHAR;
        } else if (cell.is_flagged) {
            return FALSE_FLAG_CHAR;
        } else if (cell.type == EMPTY) {
            return EMPTY_REVEALED_CHAR;
        } else {
            return ('0'+cell.type);
        }
    }
}