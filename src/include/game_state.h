/* Shared types, constants, and global state. Included by all modules. */

#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "inkview.h"
#include <stddef.h>

/* Board size is a runtime choice (see GameState.board_size), not a
   compile-time constant -- arrays are sized for the largest
   supported board (5x5) and only the active board_size x board_size
   portion is used. See docs/board_size_5x5_plan.md. */
#define MAX_BOARD_SIZE 5
#define MAX_DICE_COUNT (MAX_BOARD_SIZE * MAX_BOARD_SIZE) /* 25 */
#define DEFAULT_BOARD_SIZE 4

#define MAX_WORD_LEN 64 /* 25 cells * 2 chars (QU) + '\0', rounded up */
#define MAX_WORDS 200
#define MIN_WORD_LEN 3         /* compiled-in default; rulesets carry their own */
#define TIMER_SECONDS 180      /* 3:00 -- default and one of two options */
#define TIMER_SECONDS_SHORT 90 /* 1:30 -- the other option */

typedef enum
{
   MODE_SOLO = 0,
   MODE_MULTI = 1
} GameMode;
#define DEFAULT_MODE MODE_SOLO

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
extern ifont *font_large;
extern ifont *font_medium;
extern ifont *font_small;

extern int SW, SH;
extern int CELL_SIZE, GRID_X, GRID_Y;

/* "Mode" button hitbox, set in screen_game.c, read in input.c */
extern int mode_btn_x, mode_btn_y, mode_btn_w, mode_btn_h;

/* "Quit" button hitbox (top-right of the game HUD, next to "Mode"),
   set in screen_game.c, read in input.c. Closes the app entirely. */
extern int quit_btn_x, quit_btn_y, quit_btn_w, quit_btn_h;

/* Pause/Play button hitbox (below the grid, in-game screen), set in
   screen_game.c, read in input.c. Toggles g.paused. */
extern int pause_btn_x, pause_btn_y, pause_btn_w, pause_btn_h;

/* Board-size toggle hitboxes (mode-select screen), set in
   screen_mode_select.c, read in input.c */
extern int board4_btn_x, board4_btn_y, board4_btn_w, board4_btn_h;
extern int board5_btn_x, board5_btn_y, board5_btn_w, board5_btn_h;

/* Mode toggle hitboxes (mode-select screen). Unlike the old design,
   tapping these only changes selected_mode -- it no longer starts
   the game directly; the Start button does that. */
extern int mode_solo_btn_x, mode_solo_btn_y, mode_solo_btn_w, mode_solo_btn_h;
extern int mode_multi_btn_x, mode_multi_btn_y, mode_multi_btn_w, mode_multi_btn_h;

/* Timer-duration toggle hitboxes (mode-select screen). */
extern int timer90_btn_x, timer90_btn_y, timer90_btn_w, timer90_btn_h;
extern int timer180_btn_x, timer180_btn_y, timer180_btn_w, timer180_btn_h;

/* Start button hitbox (mode-select screen): the only control that
   actually calls game_start(). */
extern int start_btn_x, start_btn_y, start_btn_w, start_btn_h;

/* Player's chosen board size for the next game, defaults to 4x4.
   Set by input.c when the player taps the toggle, read by
   game_start() (game_logic.c) when a new game begins. */
extern int selected_board_size;

/* Player's chosen mode (Solo/Multiplayer) for the next game, set by
   the mode toggle, read when the Start button is tapped. */
extern GameMode selected_mode;

/* Player's chosen timer duration (TIMER_SECONDS or
   TIMER_SECONDS_SHORT) for the next game, set by the timer toggle,
   read by game_start(). */
extern int selected_timer_seconds;

#endif /* GAME_STATE_H */
