/* Single definition of all global state declared extern in game_state.h. */

#include "game_state.h"

GameState g;
Screen    cur_screen = SCREEN_MODE_SELECT;

ifont *font_title  = NULL;
ifont *font_large  = NULL;
ifont *font_medium = NULL;
ifont *font_small  = NULL;

int SW, SH;
int CELL_SIZE, GRID_X, GRID_Y;

int mode_btn_x, mode_btn_y, mode_btn_w, mode_btn_h;
int quit_btn_x, quit_btn_y, quit_btn_w, quit_btn_h;
int pause_btn_x, pause_btn_y, pause_btn_w, pause_btn_h;

int board4_btn_x, board4_btn_y, board4_btn_w, board4_btn_h;
int board5_btn_x, board5_btn_y, board5_btn_w, board5_btn_h;

int mode_solo_btn_x, mode_solo_btn_y, mode_solo_btn_w, mode_solo_btn_h;
int mode_multi_btn_x, mode_multi_btn_y, mode_multi_btn_w, mode_multi_btn_h;

int timer90_btn_x, timer90_btn_y, timer90_btn_w, timer90_btn_h;
int timer180_btn_x, timer180_btn_y, timer180_btn_w, timer180_btn_h;

int start_btn_x, start_btn_y, start_btn_w, start_btn_h;

int selected_board_size = DEFAULT_BOARD_SIZE;
GameMode selected_mode = DEFAULT_MODE;
int selected_timer_seconds = TIMER_SECONDS;
