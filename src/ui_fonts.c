/* Font loading and screen layout. */

#include "ui_fonts.h"
#include "game_state.h"
#include "i18n.h"

void ui_draw_button(int x, int y, int w, int h,
                    const char *label, int filled, ifont *preferred)
{
    FillArea(x, y, w, h, filled ? BLACK : WHITE);
    DrawRect(x, y, w, h, BLACK);
    SetFont(i18n_fit_font(label, w - UI_PAD, preferred), filled ? WHITE : BLACK);
    DrawTextRect(x, y, w, h, label, ALIGN_CENTER | VALIGN_MIDDLE);
}

void fonts_load(void)
{
    /* Bold face at large sizes for e-ink legibility. */
    font_title = OpenFont("LiberationSans-Bold", 64, 1);
    font_dice = OpenFont("LiberationSans-Bold", 108, 1); /* 3x font_large */
    font_large = OpenFont("LiberationSans-Bold", 36, 1);
    font_medium = OpenFont("LiberationSans-Bold", 28, 1);
    font_small = OpenFont("LiberationSans-Bold", 20, 1);
}

void fonts_free(void)
{
    if (font_title)
    {
        CloseFont(font_title);
        font_title = NULL;
    }

    if (font_dice)
    {
        CloseFont(font_dice);
        font_dice = NULL;
    }

    if (font_large)
    {
        CloseFont(font_large);
        font_large = NULL;
    }

    if (font_medium)
    {
        CloseFont(font_medium);
        font_medium = NULL;
    }

    if (font_small)
    {
        CloseFont(font_small);
        font_small = NULL;
    }
}

void layout_compute(void)
{
    SW = ScreenWidth();
    SH = ScreenHeight();
}

void layout_compute_grid(int board_size)
{
    /* Board-size-aware width budget so cells stay ~the same physical
       size on both boards (SW=758: 4x4 @ 60% and 5x5 @ 75% both ~113px). */
    int width_pct = (board_size == 5) ? 75 : 60;

    CELL_SIZE = (SW * width_pct / 100) / board_size;
    GRID_X = (SW - CELL_SIZE * board_size) / 2;
    GRID_Y = UI_HUD_H + UI_GAP;
}
