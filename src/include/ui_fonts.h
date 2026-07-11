#ifndef UI_FONTS_H
#define UI_FONTS_H

#include "inkview.h"

/* Shared layout metrics so every screen uses the same margins, tap
   sizes, and spacing instead of re-picking per-screen magic numbers.
   UI_HUD_H is proportional to the screen height (SH, from game_state.h)
   so it scales; main.c partial-updates exactly this band on a timer
   tick, so both sides agree on the HUD height. */
#define UI_MARGIN 24   /* content inset from the screen edge */
#define UI_GAP 28      /* vertical gap between stacked sections */
#define UI_PAD 16      /* horizontal text inset inside a button */
#define UI_BTN_H 64    /* primary/secondary button height */
#define UI_TOGGLE_H 60 /* setup-screen toggle height */
#define UI_HUD_H (SH * 5 / 32)

void fonts_load(void);
void fonts_free(void);
void layout_compute(void);
void layout_compute_grid(int board_size);

/* One button style for every screen: filled (black fill, white text)
   for the selected/primary action, outlined (white fill, black border
   and text) otherwise. Text is centered and shrunk to fit via
   i18n_fit_font(). Leaves a font active as a side effect. */
void ui_draw_button(int x, int y, int w, int h,
                    const char *label, int filled, ifont *preferred);

#endif // UI_FONTS_H
