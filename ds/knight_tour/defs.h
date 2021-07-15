#define BOARD_SIZE (64)
#define MAX_POS_MOVE (8)
#define ALL_BITS_ON (0xFFFFFFFFFFFFFFFF)
#define MAX_ROW_NUM (8)
#define ROW_INDEX (1)
#define MAX_COL_NUM (8)
#define COL_INDEX (2)
#define LEGIT_ROW(move_index) 0 <= cur_pos_idx / MAX_ROW_NUM + moves[ROW_INDEX][move_index] && MAX_ROW_NUM > cur_pos_idx / MAX_ROW_NUM + moves[ROW_INDEX][move_index]
#define LEGIT_COL(move_index) 0 <= cur_pos_idx % MAX_COL_NUM + moves[COL_INDEX][move_index] && MAX_COL_NUM > cur_pos_idx % MAX_COL_NUM + moves[COL_INDEX][move_index]
#define PRINT_WITH_COLOR
#define PRINT_WITHOUT_COLOR