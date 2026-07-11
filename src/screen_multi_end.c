/* Multiplayer end screen. */

#include "screen_multi_end.h"
#include "game_state.h"
#include "ui_fonts.h"
#include "i18n.h"

void draw_multi_end(void)
{
    FillArea(0, 0, SW, SH, WHITE);

    int y = 330;
    SetFont(font_title, BLACK);
    DrawTextRect(0, y, SW, 56, i18n_str(STR_TIMES_UP), ALIGN_CENTER);
    y += 56 + 30;

    SetFont(font_medium, BLACK);
    DrawTextRect(0, y, SW, 32, i18n_str(STR_COMPARE_LISTS), ALIGN_CENTER);
    y += 40;
    DrawTextRect(0, y, SW, 32, i18n_str(STR_CANCEL_RULE), ALIGN_CENTER);
    y += 32 + UI_GAP + 12;

    int wide_w = SW * 2 / 3;
    int wide_x = (SW - wide_w) / 2;

    newgame_btn_x = wide_x;
    newgame_btn_y = y;
    newgame_btn_w = wide_w;
    newgame_btn_h = UI_BTN_H;
    ui_draw_button(newgame_btn_x, newgame_btn_y, newgame_btn_w, newgame_btn_h,
                   i18n_str(STR_BTN_NEW_GAME), 1, font_large);

    changemode_btn_x = wide_x;
    changemode_btn_y = y + UI_BTN_H + 22;
    changemode_btn_w = wide_w;
    changemode_btn_h = UI_BTN_H;
    ui_draw_button(changemode_btn_x, changemode_btn_y, changemode_btn_w, changemode_btn_h,
                   i18n_str(STR_BTN_CHANGE_MODE), 0, font_medium);
}
