/* Game screen: HUD (Pause, title, Quit/Mode, timer, score), grid,
   full-width word bar, full-width found-words panel. */

#include "screen_game.h"
#include "game_state.h"
#include "game_logic.h"
#include "ui_fonts.h"
#include "i18n.h"
#include <stdio.h>
#include <string.h>

/* First y below the grid; shared by the solo word bar and the multi
   hint so the two layouts stay in sync. */
static int below_grid_y(void)
{
    return GRID_Y + g.board_size * CELL_SIZE + 20;
}

void draw_hud(void)
{
    FillArea(0, 0, SW, UI_HUD_H, WHITE);

    int row_h = 52, row_y = 14, ctrl_w = 112, gap = 12;

    pause_btn_x = UI_MARGIN;
    pause_btn_y = row_y;
    pause_btn_w = 120;
    pause_btn_h = row_h;
    ui_draw_button(pause_btn_x, pause_btn_y, pause_btn_w, pause_btn_h,
                   i18n_str(g.paused ? STR_BTN_PLAY : STR_BTN_PAUSE),
                   0, font_small);

    mode_btn_w = ctrl_w;
    mode_btn_h = row_h;
    mode_btn_x = SW - UI_MARGIN - ctrl_w;
    mode_btn_y = row_y;
    ui_draw_button(mode_btn_x, mode_btn_y, mode_btn_w, mode_btn_h,
                   i18n_str(STR_BTN_MODE), 0, font_small);

    quit_btn_w = ctrl_w;
    quit_btn_h = row_h;
    quit_btn_x = mode_btn_x - gap - ctrl_w;
    quit_btn_y = row_y;
    ui_draw_button(quit_btn_x, quit_btn_y, quit_btn_w, quit_btn_h,
                   i18n_str(STR_BTN_QUIT), 0, font_small);

    int title_x = pause_btn_x + pause_btn_w + gap;
    int title_w = quit_btn_x - gap - title_x;
    const char *title = i18n_str(g.mode == MODE_MULTI ? STR_HUD_TITLE_MULTI
                                                      : STR_HUD_TITLE_SOLO);
    SetFont(i18n_fit_font(title, title_w - UI_PAD, font_large), BLACK);
    DrawTextRect(title_x, row_y, title_w, row_h, title, ALIGN_CENTER);

    int timer_y = row_y + row_h + 6;
    char ts[16];
    snprintf(ts, sizeof(ts), "%02d:%02d", g.time_left / 60, g.time_left % 60);
    SetFont(font_title, g.time_left <= 30 ? DGRAY : BLACK);
    DrawTextRect(0, timer_y, SW, 52, ts, ALIGN_CENTER);

    if (g.mode == MODE_SOLO)
    {
        char ss[32];
        snprintf(ss, sizeof(ss), i18n_str(STR_SCORE_LABEL), g.score);
        SetFont(font_small, BLACK);
        DrawTextRect(0, timer_y + 52, SW, 26, ss, ALIGN_CENTER);
    }

    DrawLine(0, UI_HUD_H - 2, SW, UI_HUD_H - 2, BLACK);
}

void draw_grid(void)
{
    for (int r = 0; r < g.board_size; r++)
    {
        for (int c = 0; c < g.board_size; c++)
        {
            int idx = r * g.board_size + c;
            int x = GRID_X + c * CELL_SIZE;
            int y = GRID_Y + r * CELL_SIZE;

            int bg = WHITE, order = -1;
            for (int s = 0; s < g.sel_count; s++)
            {
                if (g.selected[s] == idx)
                {
                    bg = LGRAY;
                    order = s;
                    break;
                }
            }
            FillArea(x + 2, y + 2, CELL_SIZE - 4, CELL_SIZE - 4, bg);
            DrawRect(x + 2, y + 2, CELL_SIZE - 4, CELL_SIZE - 4, BLACK);

            if (g.paused)
                continue; /* letters hidden while paused */

            if (order >= 0 && g.mode == MODE_SOLO)
            {
                char os[4];
                snprintf(os, sizeof(os), "%d", order + 1);
                SetFont(font_small, DGRAY);
                DrawString(x + CELL_SIZE - 20, y + 6, os);
            }

            /* Shrink to fit: at 5x5 even one letter (or "Qu") can be
               tight in a cell. */
            const char *lbl = cell_label(r, c);
            SetFont(i18n_fit_font(lbl, CELL_SIZE - 8, font_large), BLACK);

            int lw = StringWidth(lbl);
            DrawString(x + (CELL_SIZE - lw) / 2,
                       y + (CELL_SIZE - 30) / 2, lbl);
        }
    }
}

static void draw_grid_dark(void)
{
    for (int r = 0; r < g.board_size; r++)
    {
        for (int c = 0; c < g.board_size; c++)
        {
            int x = GRID_X + c * CELL_SIZE;
            int y = GRID_Y + r * CELL_SIZE;
            FillArea(x + 2, y + 2, CELL_SIZE - 4, CELL_SIZE - 4, BLACK);
            DrawRect(x + 2, y + 2, CELL_SIZE - 4, CELL_SIZE - 4, BLACK);
        }
    }
}

static void update_grid_region(void)
{
    int side = CELL_SIZE * g.board_size;
    PartialUpdate(GRID_X, GRID_Y, side, side);
}

void grid_game_over_blink(void)
{
    for (int flash = 0; flash < GAME_OVER_BLINKS; flash++)
    {
        draw_grid();
        update_grid_region();
        draw_grid_dark();
        update_grid_region();
    }
}

void draw_word_bar(void)
{
    int bar_y = below_grid_y();
    int gap = 12;
    FillArea(0, bar_y, SW, SH - bar_y, WHITE);

    clear_btn_w = 150;
    clear_btn_h = UI_BTN_H;
    clear_btn_x = SW - UI_MARGIN - clear_btn_w;
    clear_btn_y = bar_y;

    submit_btn_w = 150;
    submit_btn_h = UI_BTN_H;
    submit_btn_x = clear_btn_x - gap - submit_btn_w;
    submit_btn_y = bar_y;

    int wb_x = UI_MARGIN;
    int wb_w = submit_btn_x - gap - wb_x;
    DrawRect(wb_x, bar_y, wb_w, UI_BTN_H, BLACK);

    char display[MAX_WORD_LEN + 6];
    snprintf(display, sizeof(display), "> %s",
             g.sel_count > 0 ? g.current_word : "...");
    SetFont(font_medium, BLACK);
    DrawTextRect(wb_x + UI_PAD, bar_y, wb_w - 2 * UI_PAD, UI_BTN_H,
                 display, ALIGN_LEFT);

    ui_draw_button(submit_btn_x, submit_btn_y, submit_btn_w, submit_btn_h,
                   i18n_str(STR_BTN_SUBMIT), 1, font_medium);
    ui_draw_button(clear_btn_x, clear_btn_y, clear_btn_w, clear_btn_h,
                   i18n_str(STR_BTN_CLEAR), 0, font_medium);
}

void draw_found_words(void)
{
    int lx = UI_MARGIN;
    int content_w = SW - 2 * UI_MARGIN;
    int fw_y = below_grid_y() + UI_BTN_H + 20;

    char hdr[32];
    snprintf(hdr, sizeof(hdr), i18n_str(STR_WORDS_HEADER), g.word_count);
    SetFont(font_small, BLACK);
    DrawTextRect(lx, fw_y, content_w, 24, hdr, ALIGN_LEFT);

    int cols = 3, row_h = 26;
    int col_w = content_w / cols;
    int list_y = fw_y + 30;
    int rows = (SH - UI_MARGIN - list_y) / row_h;
    if (rows < 1)
        rows = 1;

    int capacity = rows * cols;
    int start = (g.word_count > capacity) ? g.word_count - capacity : 0;

    SetFont(font_small, BLACK);
    for (int i = 0; start + i < g.word_count; i++)
    {
        int wl = (int)strlen(g.found_words[start + i]);
        char line[MAX_WORD_LEN + 8];
        snprintf(line, sizeof(line), "%s +%d",
                 g.found_words[start + i], score_for_word(wl));
        DrawString(lx + (i / rows) * col_w, list_y + (i % rows) * row_h, line);
    }
}

void draw_game(void)
{
    FillArea(0, 0, SW, SH, WHITE);
    draw_hud();
    draw_grid();

    if (g.mode == MODE_SOLO)
    {
        draw_word_bar();
        draw_found_words();
    }
    else
    {
        SetFont(font_small, DGRAY);
        DrawTextRect(0, below_grid_y(), SW, 32,
                     i18n_str(STR_MULTI_INGAME_HINT), ALIGN_CENTER);
    }

    FullUpdate();
}
