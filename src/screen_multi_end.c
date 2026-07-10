/* Multiplayer end screen. */

#include "screen_multi_end.h"
#include "game_state.h"
#include "i18n.h"

void draw_multi_end(void) {
    FillArea(0, 0, SW, SH, WHITE);

    SetFont(font_title, BLACK);
    DrawTextRect(0, SH / 8, SW, 52, i18n_str(STR_TIMES_UP), ALIGN_CENTER);

    SetFont(font_medium, BLACK);
    DrawTextRect(0, SH / 8 + 70, SW, 32,
        i18n_str(STR_COMPARE_LISTS), ALIGN_CENTER);
    DrawTextRect(0, SH / 8 + 108, SW, 32,
        i18n_str(STR_CANCEL_RULE), ALIGN_CENTER);

    int bw = SW * 2 / 3, bh = 58, bx = (SW - bw) / 2;
    int replay_y = SH * 3 / 5;
    DrawRect(bx, replay_y, bw, bh, BLACK);
    const char *new_game_label = i18n_str(STR_BTN_NEW_GAME);
    SetFont(i18n_fit_font(new_game_label, bw - 20, font_large), BLACK);
    DrawTextRect(bx, replay_y + 10, bw, bh, new_game_label, ALIGN_CENTER);

    int sm_w = SW / 2, sm_h = 40, sm_x = (SW - sm_w) / 2;
    int sm_y = replay_y + bh + 18;
    DrawRect(sm_x, sm_y, sm_w, sm_h, DGRAY);
    const char *change_mode_label = i18n_str(STR_BTN_CHANGE_MODE);
    SetFont(i18n_fit_font(change_mode_label, sm_w - 20, font_medium), DGRAY);
    DrawTextRect(sm_x, sm_y + 8, sm_w, sm_h, change_mode_label, ALIGN_CENTER);
}
