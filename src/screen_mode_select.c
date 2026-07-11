/* Setup screen: Mode/Board-size/Timer toggles + Start. Toggles only
   change the selection; only Start calls game_start(). */

#include "screen_mode_select.h"
#include "game_state.h"
#include "ui_fonts.h"
#include "i18n.h"

/* A centered caption over two equal-width buttons at the shared pair
   width, so all three rows line up. */
static int draw_toggle_row(int y, const char *caption,
                           const char *left, int left_sel,
                           const char *right, int right_sel,
                           int *lx, int *rx)
{
   int pair_w = SW * 2 / 3, gap = 16;
   int half = (pair_w - gap) / 2;
   int px = (SW - pair_w) / 2;

   SetFont(font_small, DGRAY);
   DrawTextRect(0, y, SW, 24, caption, ALIGN_CENTER);
   y += 24 + 8;

   *lx = px;
   *rx = px + half + gap;
   ui_draw_button(*lx, y, half, UI_TOGGLE_H, left, left_sel, font_large);
   ui_draw_button(*rx, y, half, UI_TOGGLE_H, right, right_sel, font_large);

   return y;
}

void draw_mode_select(void)
{
   FillArea(0, 0, SW, SH, WHITE);

   int y = 90;
   SetFont(font_title, BLACK);
   DrawTextRect(0, y, SW, 56, "BOOKGGLE", ALIGN_CENTER);
   y += 56 + 18;

   SetFont(font_medium, BLACK);
   DrawTextRect(0, y, SW, 32, i18n_str(STR_CHOOSE_MODE), ALIGN_CENTER);
   y += 32 + 50;

   int lx, rx, row_y;
   int half = (SW * 2 / 3 - 16) / 2;

   row_y = draw_toggle_row(y, i18n_str(STR_BTN_MODE),
                           i18n_str(STR_BTN_SOLO), selected_mode == MODE_SOLO,
                           i18n_str(STR_BTN_MULTIPLAYER), selected_mode == MODE_MULTI,
                           &lx, &rx);
   mode_solo_btn_x = lx;
   mode_solo_btn_y = row_y;
   mode_solo_btn_w = half;
   mode_solo_btn_h = UI_TOGGLE_H;
   mode_multi_btn_x = rx;
   mode_multi_btn_y = row_y;
   mode_multi_btn_w = half;
   mode_multi_btn_h = UI_TOGGLE_H;
   y = row_y + UI_TOGGLE_H + 40;

   /* Digits / "x" / times aren't prose, so kept as literals rather
      than routed through i18n. */
   row_y = draw_toggle_row(y, i18n_str(STR_LABEL_BOARD_SIZE),
                           "4x4", selected_board_size == 4,
                           "5x5", selected_board_size == 5,
                           &lx, &rx);
   board4_btn_x = lx;
   board4_btn_y = row_y;
   board4_btn_w = half;
   board4_btn_h = UI_TOGGLE_H;
   board5_btn_x = rx;
   board5_btn_y = row_y;
   board5_btn_w = half;
   board5_btn_h = UI_TOGGLE_H;
   y = row_y + UI_TOGGLE_H + 40;

   row_y = draw_toggle_row(y, i18n_str(STR_LABEL_TIMER),
                           "1:30", selected_timer_seconds == TIMER_SECONDS_SHORT,
                           "3:00", selected_timer_seconds == TIMER_SECONDS,
                           &lx, &rx);
   timer90_btn_x = lx;
   timer90_btn_y = row_y;
   timer90_btn_w = half;
   timer90_btn_h = UI_TOGGLE_H;
   timer180_btn_x = rx;
   timer180_btn_y = row_y;
   timer180_btn_w = half;
   timer180_btn_h = UI_TOGGLE_H;
   y = row_y + UI_TOGGLE_H + 56;

   int wide_w = SW * 2 / 3;
   int wide_x = (SW - wide_w) / 2;

   start_btn_x = wide_x;
   start_btn_y = y;
   start_btn_w = wide_w;
   start_btn_h = UI_BTN_H;
   ui_draw_button(start_btn_x, start_btn_y, start_btn_w, start_btn_h,
                  i18n_str(STR_BTN_START), 1, font_large);

   quit_btn_x = wide_x;
   quit_btn_y = y + UI_BTN_H + 22;
   quit_btn_w = wide_w;
   quit_btn_h = UI_BTN_H;
   ui_draw_button(quit_btn_x, quit_btn_y, quit_btn_w, quit_btn_h,
                  i18n_str(STR_BTN_QUIT), 0, font_medium);

   FullUpdate();
}
