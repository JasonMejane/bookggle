/* Game screen: HUD (Pause, title, Quit/Mode, timer, score), grid,
   word bar, found-words panel. */

#ifndef SCREEN_GAME_H
#define SCREEN_GAME_H

/* HUD only — used for partial redraw on each timer tick. Also sets the
   Pause/Play, Mode, and Quit button hitboxes. */
void draw_hud(void);

void draw_grid(void);

/* Number of dark<->normal flashes the grid shows when the game ends,
   before it settles fully dark. */
#define GAME_OVER_BLINKS 2

/* Blinks the grid GAME_OVER_BLINKS times and leaves every die filled
   solid black to signal the game has ended. Blocking (each frame is an
   e-ink refresh over the grid region); meant to run once at game over. */
void grid_game_over_blink(void);

void draw_word_bar(void);
void draw_found_words(void);

/* Full game screen, triggers FullUpdate. */
void draw_game(void);

#endif /* SCREEN_GAME_H */
