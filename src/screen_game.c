/* Game screen: HUD (timer, score, Mode/Quit), grid, Pause/Play
   button, word bar, word list. */

#include "screen_game.h"
#include "game_state.h"
#include "game_logic.h"
#include "i18n.h"
#include <stdio.h>
#include <string.h>

void draw_hud(void) {
    int hud_h = SH / 5;
    FillArea(0, 0, SW, hud_h, WHITE);

    SetFont(font_title, BLACK);
    const char *title = i18n_str(g.mode == MODE_MULTI ? STR_HUD_TITLE_MULTI : STR_HUD_TITLE_SOLO);
    DrawTextRect(0, 6, SW, 44, title, ALIGN_CENTER);

    char ts[16];
    snprintf(ts, sizeof(ts), "%02d:%02d", g.time_left / 60, g.time_left % 60);
    SetFont(font_large, g.time_left <= 30 ? DGRAY : BLACK);
    DrawTextRect(0, 54, SW, 36, ts, ALIGN_CENTER);

    if (g.mode == MODE_SOLO) {
        char ss[32];
        snprintf(ss, sizeof(ss), i18n_str(STR_SCORE_LABEL), g.score);
        SetFont(font_small, BLACK);
        DrawTextRect(0, 94, SW / 2, 22, ss, ALIGN_CENTER);
    }

    /* Top-right corner: two buttons, Quit (far corner) and Mode
       (just to its left). */
    mode_btn_w = 80; mode_btn_h = 28;
    mode_btn_x = SW - mode_btn_w - 6;
    mode_btn_y = 8;
    DrawRect(mode_btn_x, mode_btn_y, mode_btn_w, mode_btn_h, DGRAY);
    const char *mode_label = i18n_str(STR_BTN_MODE);
    SetFont(i18n_fit_font(mode_label, mode_btn_w - 8, font_small), DGRAY);
    DrawTextRect(mode_btn_x, mode_btn_y + 4,
                 mode_btn_w, mode_btn_h, mode_label, ALIGN_CENTER);

    quit_btn_w = 80; quit_btn_h = 28;
    quit_btn_x = mode_btn_x - quit_btn_w - 10;
    quit_btn_y = 8;
    DrawRect(quit_btn_x, quit_btn_y, quit_btn_w, quit_btn_h, DGRAY);
    const char *quit_label = i18n_str(STR_BTN_QUIT);
    SetFont(i18n_fit_font(quit_label, quit_btn_w - 8, font_small), DGRAY);
    DrawTextRect(quit_btn_x, quit_btn_y + 4,
                 quit_btn_w, quit_btn_h, quit_label, ALIGN_CENTER);

    DrawLine(0, hud_h - 2, SW, hud_h - 2, BLACK);
}

void draw_grid(void) {
    for (int r = 0; r < g.board_size; r++) {
        for (int c = 0; c < g.board_size; c++) {
            int idx = r * g.board_size + c;
            int x = GRID_X + c * CELL_SIZE;
            int y = GRID_Y + r * CELL_SIZE;

            int bg = WHITE, order = -1;
            for (int s = 0; s < g.sel_count; s++) {
                if (g.selected[s] == idx) {
                    bg = LGRAY; order = s; break;
                }
            }
            FillArea(x + 2, y + 2, CELL_SIZE - 4, CELL_SIZE - 4, bg);
            DrawRect(x + 2, y + 2, CELL_SIZE - 4, CELL_SIZE - 4, BLACK);

            if (g.paused) continue; /* letters hidden while paused */

            if (order >= 0 && g.mode == MODE_SOLO) {
                char os[4];
                snprintf(os, sizeof(os), "%d", order + 1);
                SetFont(font_small, DGRAY);
                DrawString(x + CELL_SIZE - 20, y + 6, os);
            }

            /* Measure-and-shrink instead of a fixed Q-vs-other
               heuristic: at smaller cell sizes (5x5) even a single
               letter can get tight, and this reuses the same fit
               logic already used for translated button labels. */
            const char *lbl = cell_label(r, c);
            SetFont(i18n_fit_font(lbl, CELL_SIZE - 8, font_large), BLACK);

            int lw = StringWidth(lbl);
            DrawString(x + (CELL_SIZE - lw) / 2,
                       y + (CELL_SIZE - 30) / 2, lbl);
        }
    }
}

/* Y coordinate just below the grid, shared by the Pause/Play button
   and whatever sits below it (word bar or multi-mode hint) so the
   two layouts can't drift out of sync with each other. */
static int below_grid_y(void) {
    return GRID_Y + g.board_size * CELL_SIZE + 14;
}

void draw_pause_button(void) {
    pause_btn_w = 160; pause_btn_h = 44;
    pause_btn_x = (SW - pause_btn_w) / 2;
    pause_btn_y = below_grid_y();

    DrawRect(pause_btn_x, pause_btn_y, pause_btn_w, pause_btn_h, BLACK);
    const char *label = i18n_str(g.paused ? STR_BTN_PLAY : STR_BTN_PAUSE);
    SetFont(i18n_fit_font(label, pause_btn_w - 16, font_medium), BLACK);
    DrawTextRect(pause_btn_x, pause_btn_y + 8,
                 pause_btn_w, pause_btn_h - 8, label, ALIGN_CENTER);
}

void draw_word_bar(void) {
    int bar_y = pause_btn_y + pause_btn_h + 14;
    FillArea(0, bar_y, SW, SH - bar_y, WHITE);

    char display[MAX_WORD_LEN + 6];
    snprintf(display, sizeof(display), "> %s",
             g.sel_count > 0 ? g.current_word : "...");
    SetFont(font_medium, BLACK);
    DrawTextRect(0, bar_y + 8, SW - 114, 36, display, ALIGN_CENTER);

    int bx = SW - 110, by = bar_y + 6;
    DrawRect(bx, by, 100, 36, BLACK);
    const char *submit_label = i18n_str(STR_BTN_SUBMIT);
    SetFont(i18n_fit_font(submit_label, 92, font_medium), BLACK);
    DrawTextRect(bx, by + 6, 100, 36, submit_label, ALIGN_CENTER);

    DrawRect(bx, by + 44, 100, 32, DGRAY);
    const char *clear_label = i18n_str(STR_BTN_CLEAR);
    SetFont(i18n_fit_font(clear_label, 92, font_small), DGRAY);
    DrawTextRect(bx, by + 50, 100, 32, clear_label, ALIGN_CENTER);
}

void draw_found_words(void) {
    int lx = GRID_X + g.board_size * CELL_SIZE + 10;
    int ly = GRID_Y;
    int lw = SW - lx - 6;

    char hdr[32];
    snprintf(hdr, sizeof(hdr), i18n_str(STR_WORDS_HEADER), g.word_count);
    SetFont(font_small, BLACK);
    DrawTextRect(lx, ly, lw, 22, hdr, ALIGN_LEFT);

    int y = ly + 26;
    int start = (g.word_count > 14) ? g.word_count - 14 : 0;
    for (int i = start; i < g.word_count; i++) {
        int wl = (int)strlen(g.found_words[i]);
        char line[MAX_WORD_LEN + 8];
        snprintf(line, sizeof(line), "%s +%d",
                 g.found_words[i], score_for_word(wl));
        DrawString(lx, y, line);
        y += 20;
        if (y > SH - 20) break;
    }
}

void draw_game(void) {
    FillArea(0, 0, SW, SH, WHITE);
    draw_hud();
    draw_grid();
    draw_pause_button();

    if (g.mode == MODE_SOLO) {
        draw_word_bar();
        draw_found_words();
    } else {
        int msg_y = pause_btn_y + pause_btn_h + 16;
        SetFont(font_small, DGRAY);
        DrawTextRect(0, msg_y, SW, 28,
            i18n_str(STR_MULTI_INGAME_HINT),
            ALIGN_CENTER);
    }
    FullUpdate();
}
