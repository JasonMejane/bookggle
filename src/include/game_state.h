/* Shared types, constants, and global state. Included by all modules. */

#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "inkview.h"
#include <stddef.h>

/* Board size is a runtime choice (GameState.board_size), not
   compile-time: arrays are sized for the largest board and only the
   active board_size x board_size slice is used. */
#define MAX_BOARD_SIZE 5
#define MAX_DICE_COUNT (MAX_BOARD_SIZE * MAX_BOARD_SIZE)
#define DEFAULT_BOARD_SIZE 4

#define MAX_WORD_LEN 64 /* 25 cells * 2 chars (QU) + '\0', rounded up */
#define MAX_WORDS 200
#define MIN_WORD_LEN 3         /* compiled-in default; rulesets carry their own */
#define TIMER_SECONDS 180      /* 3:00 */
#define TIMER_SECONDS_SHORT 90 /* 1:30 */

typedef enum
{
   MODE_SOLO = 0,
   MODE_MULTI = 1
} GameMode;
#define DEFAULT_MODE MODE_MULTI

typedef enum
{
   SCREEN_MODE_SELECT,
   SCREEN_GAME,
   SCREEN_MULTI_END
} Screen;

typedef struct
{
   char grid[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
   int selected[MAX_DICE_COUNT];
   int board_size; /* 4 or 5, set by game_start() */
   int sel_count;
   char current_word[MAX_WORD_LEN];
   char found_words[MAX_WORDS][MAX_WORD_LEN];
   int word_count;
   int score;
   int time_left;
   int game_over;
   int paused; /* grid hidden, timer frozen; see timer.c/game_logic.c */
   GameMode mode;
} GameState;

/* Defined once in game_state.c */
extern GameState g;
extern Screen cur_screen;

extern ifont *font_title;
extern ifont *font_dice;
extern ifont *font_large;
extern ifont *font_medium;
extern ifont *font_small;

extern int SW, SH;
extern int CELL_SIZE, GRID_X, GRID_Y;

/* Button hitboxes: the screen that draws each control writes its
   *_btn_x/y/w/h here; input.c reads them to route taps. Game HUD: */
extern int mode_btn_x, mode_btn_y, mode_btn_w, mode_btn_h;
extern int quit_btn_x, quit_btn_y, quit_btn_w, quit_btn_h;
extern int pause_btn_x, pause_btn_y, pause_btn_w, pause_btn_h;
extern int submit_btn_x, submit_btn_y, submit_btn_w, submit_btn_h;
extern int clear_btn_x, clear_btn_y, clear_btn_w, clear_btn_h;

/* Multiplayer end screen: */
extern int newgame_btn_x, newgame_btn_y, newgame_btn_w, newgame_btn_h;
extern int changemode_btn_x, changemode_btn_y, changemode_btn_w, changemode_btn_h;

/* Mode-select toggles + Start: */
extern int board4_btn_x, board4_btn_y, board4_btn_w, board4_btn_h;
extern int board5_btn_x, board5_btn_y, board5_btn_w, board5_btn_h;
extern int mode_solo_btn_x, mode_solo_btn_y, mode_solo_btn_w, mode_solo_btn_h;
extern int mode_multi_btn_x, mode_multi_btn_y, mode_multi_btn_w, mode_multi_btn_h;
extern int timer90_btn_x, timer90_btn_y, timer90_btn_w, timer90_btn_h;
extern int timer180_btn_x, timer180_btn_y, timer180_btn_w, timer180_btn_h;
extern int start_btn_x, start_btn_y, start_btn_w, start_btn_h;

/* Chosen settings for the next game: written by the mode-select
   toggles, applied to g by game_start(). */
extern int selected_board_size;
extern GameMode selected_mode;
extern int selected_timer_seconds;

#endif /* GAME_STATE_H */
