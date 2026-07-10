/* Game screen: HUD (timer, score, Mode/Quit), grid, Pause/Play
   button, word bar, word list. */

#ifndef SCREEN_GAME_H
#define SCREEN_GAME_H

/* HUD only — used for partial redraw on each timer tick. */
void draw_hud(void);

void draw_grid(void);

/* Sets pause_btn_x/y/w/h. Label is "Pause" or "Play" depending on
   g.paused. Must be called before draw_word_bar() (solo) or the
   multi-mode hint text in draw_game(), since both position
   themselves below this button. */
void draw_pause_button(void);

void draw_word_bar(void);
void draw_found_words(void);

/* Full game screen, triggers FullUpdate. */
void draw_game(void);

#endif /* SCREEN_GAME_H */
