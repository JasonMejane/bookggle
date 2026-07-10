/* Font loading and screen layout. */

#include "ui_fonts.h"
#include "game_state.h"

void fonts_load(void)
{
    /* Make the UI text noticeably larger on the target e-reader and
       use the bold face for the shared fonts so dice letters and other
       labels render more prominently. */
    font_title = OpenFont("LiberationSans-Bold", 48, 1);
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
    /* Width budget is board-size-aware so touch targets stay close
       to the same physical size at 5x5 as at 4x4, rather than
       shrinking by an extra column's worth of room. At SW=758:
       4x4 @ 60% -> 113px cells; 5x5 @ 75% -> 113px cells too. */
    int width_pct = (board_size == 5) ? 75 : 60;

    CELL_SIZE = (SW * width_pct / 100) / board_size;
    GRID_X = (SW - CELL_SIZE * board_size) / 2;
    GRID_Y = SH / 5;
}
