/* Setup screen: mode / board-size / timer toggles, then Start.
   Toggles only change the player's selection (selected_mode,
   selected_board_size, selected_timer_seconds); only the Start
   button actually calls game_start(). */

#include "screen_mode_select.h"
#include "game_state.h"
#include "i18n.h"

/* Filled = currently selected, outline = not. Shared by all three
   toggle pairs below so their look/feel stays consistent and the
   fit-font measurement logic isn't duplicated three times. */
static void draw_toggle_button(int x, int y, int w, int h,
                               const char *label, int selected,
                               ifont *preferred_font)
{
   int fill = selected ? BLACK : WHITE;
   int text_color = selected ? WHITE : BLACK;
   FillArea(x, y, w, h, fill);
   DrawRect(x, y, w, h, BLACK);
   ifont *f = i18n_fit_font(label, w - 12, preferred_font);
   SetFont(f, text_color);
   DrawTextRect(x, y + 8, w, h - 8, label, ALIGN_CENTER);
}

void draw_mode_select(void)
{
   FillArea(0, 0, SW, SH, WHITE);

   SetFont(font_title, BLACK);
   DrawTextRect(0, SH / 8, SW, 52, "BOOKGGLE", ALIGN_CENTER);

   SetFont(font_medium, BLACK);
   DrawTextRect(0, SH / 8 + 62, SW, 30,
                i18n_str(STR_CHOOSE_MODE), ALIGN_CENTER);

   int y = SH / 8 + 62 + 30 + 30;

   /* --- Mode: Solo / Multiplayer --- */
   SetFont(font_small, DGRAY);
   DrawTextRect(0, y, SW, 22, i18n_str(STR_BTN_MODE), ALIGN_CENTER);
   y += 22 + 8;

   int mode_pair_w = SW * 2 / 3, mode_gap = 20;
   int mode_w = (mode_pair_w - mode_gap) / 2, mode_h = 56;
   int mode_x = (SW - mode_pair_w) / 2;

   mode_solo_btn_x = mode_x;
   mode_solo_btn_y = y;
   mode_solo_btn_w = mode_w;
   mode_solo_btn_h = mode_h;
   mode_multi_btn_x = mode_x + mode_w + mode_gap;
   mode_multi_btn_y = y;
   mode_multi_btn_w = mode_w;
   mode_multi_btn_h = mode_h;

   draw_toggle_button(mode_solo_btn_x, mode_solo_btn_y, mode_w, mode_h,
                      i18n_str(STR_BTN_SOLO), selected_mode == MODE_SOLO,
                      font_large);
   draw_toggle_button(mode_multi_btn_x, mode_multi_btn_y, mode_w, mode_h,
                      i18n_str(STR_BTN_MULTIPLAYER), selected_mode == MODE_MULTI,
                      font_large);

   y += mode_h + 34;

   /* --- Board size: 4x4 / 5x5 --- */
   SetFont(font_small, DGRAY);
   DrawTextRect(0, y, SW, 22, i18n_str(STR_LABEL_BOARD_SIZE), ALIGN_CENTER);
   y += 22 + 8;

   int board_w = 90, board_h = 40, board_gap = 20;
   int board_total_w = board_w * 2 + board_gap;
   int board_x = (SW - board_total_w) / 2;

   board4_btn_x = board_x;
   board4_btn_y = y;
   board4_btn_w = board_w;
   board4_btn_h = board_h;
   board5_btn_x = board_x + board_w + board_gap;
   board5_btn_y = y;
   board5_btn_w = board_w;
   board5_btn_h = board_h;

   /* Digits + "x" aren't prose, so left as a plain literal like
      "BOOKGGLE" above rather than routed through i18n. */
   draw_toggle_button(board4_btn_x, board4_btn_y, board_w, board_h,
                      "4x4", selected_board_size == 4, font_medium);
   draw_toggle_button(board5_btn_x, board5_btn_y, board_w, board_h,
                      "5x5", selected_board_size == 5, font_medium);

   y += board_h + 34;

   /* --- Timer: 1:30 / 3:00 --- */
   SetFont(font_small, DGRAY);
   DrawTextRect(0, y, SW, 22, i18n_str(STR_LABEL_TIMER), ALIGN_CENTER);
   y += 22 + 8;

   int timer_w = 100, timer_h = 40, timer_gap = 20;
   int timer_total_w = timer_w * 2 + timer_gap;
   int timer_x = (SW - timer_total_w) / 2;

   timer90_btn_x = timer_x;
   timer90_btn_y = y;
   timer90_btn_w = timer_w;
   timer90_btn_h = timer_h;
   timer180_btn_x = timer_x + timer_w + timer_gap;
   timer180_btn_y = y;
   timer180_btn_w = timer_w;
   timer180_btn_h = timer_h;

   /* Time displays, same treatment as the board-size digits above. */
   draw_toggle_button(timer90_btn_x, timer90_btn_y, timer_w, timer_h,
                      "1:30", selected_timer_seconds == TIMER_SECONDS_SHORT,
                      font_medium);
   draw_toggle_button(timer180_btn_x, timer180_btn_y, timer_w, timer_h,
                      "3:00", selected_timer_seconds == TIMER_SECONDS,
                      font_medium);

   y += timer_h + 50;

   /* --- Start --- */
   int start_w = SW * 2 / 3, start_h = 64;
   int start_x = (SW - start_w) / 2;

   start_btn_x = start_x;
   start_btn_y = y;
   start_btn_w = start_w;
   start_btn_h = start_h;

   FillArea(start_x, y, start_w, start_h, BLACK);
   DrawRect(start_x, y, start_w, start_h, BLACK);
   const char *start_label = i18n_str(STR_BTN_START);
   SetFont(i18n_fit_font(start_label, start_w - 20, font_large), WHITE);
   DrawTextRect(start_x, y + 10, start_w, start_h, start_label, ALIGN_CENTER);

   /* Quit button placed beneath Start so the mode-select screen has
      a clear exit path without leaving the current screen layout. */
   quit_btn_x = start_x;
   quit_btn_y = y + start_h + 16;
   quit_btn_w = start_w;
   quit_btn_h = 48;

   FillArea(quit_btn_x, quit_btn_y, quit_btn_w, quit_btn_h, BLACK);
   DrawRect(quit_btn_x, quit_btn_y, quit_btn_w, quit_btn_h, BLACK);
   const char *quit_label = i18n_str(STR_BTN_QUIT);
   SetFont(i18n_fit_font(quit_label, quit_btn_w - 20, font_medium), WHITE);
   DrawTextRect(quit_btn_x, quit_btn_y + 6, quit_btn_w, quit_btn_h,
                quit_label, ALIGN_CENTER);

   FullUpdate();
}
