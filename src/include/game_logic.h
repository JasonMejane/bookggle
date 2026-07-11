/* Game rules: grid, touch selection, words, score. No drawing here. */

#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "game_state.h"

void game_reset_selection(void);

/* Rolls a new grid, resets score/words/timer, switches to SCREEN_GAME,
   (re)starts the timer. */
void game_start(GameMode mode);

/* Ends the in-progress game like a timeout would (stops the timer
   from re-arming, see game_timer_callback), without switching screens
   itself -- for when the player navigates away mid-game. */
void game_cancel(void);

/* "Qu" for a Q cell, single letter otherwise. */
const char *cell_label(int row, int col);

/* "QU" for a Q cell, single letter otherwise. */
void cell_contrib(int row, int col, char *out, int maxout);

int cell_from_coords(int x, int y, int *row, int *col);
int is_adjacent(int idx1, int idx2);
int already_selected(int idx);

/* Touch on (x, y): adds/removes a cell from the path, rebuilds the
   word. No-op in multi mode or after game over. Caller redraws. */
void handle_cell_touch(int x, int y);

int word_already_found(const char *w);

/* Length-only check (no dictionary). */
int word_is_valid(const char *w);

int score_for_word(int len);

/* Checks length, duplicate, validity; updates score and word list;
   resets selection. No-op in multi mode. Caller redraws. */
void validate_word(void);

#endif /* GAME_LOGIC_H */
