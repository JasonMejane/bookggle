/* Full game scenario, calls real module functions (no mocks).
   Run: make test-flow (from tests/), or see README.md. */

#include "game_state.h"
#include "ui_fonts.h"
#include "game_logic.h"
#include "timer.h"
#include "screen_mode_select.h"
#include "screen_game.h"
#include "screen_multi_end.h"
#include "input.h"
#include "i18n.h"
#include "ruleset.h"
#include "stub_hooks.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

/* timer.c needs this symbol to link; real version (with drawing)
   lives in src/main.c, not built into this test binary. */
void on_timer_tick(int id)
{
   (void)game_timer_callback(id);
}

int main(void)
{
   /* init */
   fonts_load();
   layout_compute();
   i18n_init();
   ruleset_select(DEFAULT_BOARD_SIZE);
   cur_screen = SCREEN_MODE_SELECT;
   printf("[OK] init: SW=%d SH=%d\n", SW, SH);

   /* Draw once so every toggle/Start hitbox global is populated,
      mirroring what a real EVT_SHOW would do -- from here on, taps
      use those globals directly instead of recomputing layout math. */
   draw_mode_select();

   /* Tapping Solo only selects it -- the game must NOT start yet. */
   input_handle_pointerup(mode_solo_btn_x + mode_solo_btn_w / 2,
                          mode_solo_btn_y + mode_solo_btn_h / 2);
   assert(selected_mode == MODE_SOLO);
   assert(cur_screen == SCREEN_MODE_SELECT);
   printf("[OK] tap Solo toggle -> selected_mode=%d, still on setup screen\n",
          selected_mode);

   /* Start actually begins the game with the current selections. */
   input_handle_pointerup(start_btn_x + start_btn_w / 2,
                          start_btn_y + start_btn_h / 2);
   assert(cur_screen == SCREEN_GAME);
   assert(g.mode == MODE_SOLO);
   assert(g.board_size == DEFAULT_BOARD_SIZE);
   assert(g.time_left == TIMER_SECONDS);
   /* game_start() (triggered by Start above) has now called
      layout_compute_grid(), so CELL_SIZE/GRID_X/GRID_Y are valid. */
   printf("[OK] tap Start -> SCREEN_GAME, mode=%d, board_size=%d, "
          "time_left=%d, CELL=%d\n",
          g.mode, g.board_size, g.time_left, CELL_SIZE);

   /* deterministic grid, Q forced at (0,0) for the QU digraph check */
   for (int r = 0; r < g.board_size; r++)
      for (int c = 0; c < g.board_size; c++)
         g.grid[r][c] = (char)('A' + r * g.board_size + c);
   g.grid[0][0] = 'Q';

   int x0 = GRID_X + 0 * CELL_SIZE + CELL_SIZE / 2;
   int y0 = GRID_Y + 0 * CELL_SIZE + CELL_SIZE / 2;
   int x1 = GRID_X + 1 * CELL_SIZE + CELL_SIZE / 2;
   int y1 = GRID_Y + 0 * CELL_SIZE + CELL_SIZE / 2;

   handle_cell_touch(x0, y0);
   printf("[OK] touch cell0 (Q) -> current_word='%s'\n", g.current_word);
   assert(strcmp(g.current_word, "QU") == 0);

   handle_cell_touch(x1, y1);
   printf("[OK] touch cell1 (B) -> current_word='%s'\n", g.current_word);
   assert(strcmp(g.current_word, "QUB") == 0);

   /* word validation: score + word list */
   int word_count_before = g.word_count;
   int score_before = g.score;
   validate_word();
   assert(g.word_count == word_count_before + 1);
   assert(g.score == score_before + score_for_word(3));
   assert(g.sel_count == 0);
   printf("[OK] validate_word -> word_count=%d score=%d\n",
          g.word_count, g.score);

   /* duplicate word detection */
   handle_cell_touch(x0, y0);
   handle_cell_touch(x1, y1);
   int wc2 = g.word_count;
   validate_word();
   assert(g.word_count == wc2);
   printf("[OK] duplicate rejected, word_count unchanged (%d)\n", g.word_count);

   /* Pause: tap the Pause/Play button (draw_game() from the Start
      tap earlier already populated pause_btn_x/y/w/h). */
   assert(g.paused == 0);
   input_handle_pointerup(pause_btn_x + pause_btn_w / 2,
                          pause_btn_y + pause_btn_h / 2);
   assert(g.paused == 1);
   printf("[OK] tap Pause/Play button -> g.paused=%d\n", g.paused);

   /* Timer is frozen while paused: tick happens, time_left doesn't
      move, and no TIMER_TICK_ENDED_* is ever returned just because
      ticks kept arriving while paused. */
   int frozen_time = g.time_left;
   TimerTickResult r_paused = game_timer_callback(0);
   assert(r_paused == TIMER_TICK_RUNNING);
   assert(g.time_left == frozen_time);
   printf("[OK] timer tick while paused -> time_left unchanged (%d)\n",
          g.time_left);

   /* Cell touch is a no-op while paused -- grid data itself is
      untouched (still 'Q' at (0,0)), only drawing/input are gated. */
   int sel_before_pause_touch = g.sel_count;
   handle_cell_touch(x0, y0);
   assert(g.sel_count == sel_before_pause_touch);
   assert(g.grid[0][0] == 'Q');
   printf("[OK] cell touch ignored while paused (sel_count=%d)\n", g.sel_count);

   /* Resume: tap the same button again -> now shows "Play" was
      tapped, flips back to running, and the timer resumes counting
      down from exactly where it was frozen. */
   input_handle_pointerup(pause_btn_x + pause_btn_w / 2,
                          pause_btn_y + pause_btn_h / 2);
   assert(g.paused == 0);
   TimerTickResult r_resumed = game_timer_callback(0);
   assert(r_resumed == TIMER_TICK_RUNNING);
   assert(g.time_left == frozen_time - 1);
   printf("[OK] tap Pause/Play again -> g.paused=%d, timer resumes at "
          "time_left=%d (from frozen %d)\n",
          g.paused, g.time_left, frozen_time);

   /* Quit: tap the HUD's Quit button, verify it actually reaches
      CloseApp() (counted by the stub -- the real SDK would exit the
      app here, the stub just counts so this is observable). */
   int close_calls_before = stub_close_app_call_count();
   input_handle_pointerup(quit_btn_x + quit_btn_w / 2,
                          quit_btn_y + quit_btn_h / 2);
   assert(stub_close_app_call_count() == close_calls_before + 1);
   printf("[OK] tap Quit button -> CloseApp() called (count=%d)\n",
          stub_close_app_call_count());

   /* timer down to 0:00 in solo mode */
   g.time_left = 2;
   g.game_over = 0;
   TimerTickResult r1 = game_timer_callback(0);
   assert(r1 == TIMER_TICK_RUNNING);
   assert(g.time_left == 1 && !g.game_over);
   TimerTickResult r2 = game_timer_callback(0);
   assert(r2 == TIMER_TICK_ENDED_SOLO);
   assert(g.time_left == 0 && g.game_over);
   printf("[OK] solo timer -> TIMER_TICK_ENDED_SOLO, game_over=%d at 0:00\n",
          g.game_over);

   /* timer down to 0:00 in multi mode -> grid hidden */
   game_start(MODE_MULTI);
   assert(cur_screen == SCREEN_GAME);
   assert(g.mode == MODE_MULTI);
   g.time_left = 1;
   TimerTickResult r3 = game_timer_callback(0);
   assert(r3 == TIMER_TICK_ENDED_MULTI);
   assert(g.time_left == 0 && g.game_over);
   assert(cur_screen == SCREEN_MULTI_END);
   printf("[OK] multi timer -> TIMER_TICK_ENDED_MULTI, "
          "cur_screen=SCREEN_MULTI_END (grid hidden)\n");

   /* touch ignored in multi mode */
   game_start(MODE_MULTI);
   handle_cell_touch(x0, y0);
   assert(g.sel_count == 0);
   printf("[OK] touch ignored in multi mode (sel_count=%d)\n", g.sel_count);

   /* "Mode" HUD button returns to setup screen */
   draw_game(); /* sets mode_btn_x/y/w/h for this tap */
   input_handle_pointerup(mode_btn_x + mode_btn_w / 2,
                          mode_btn_y + mode_btn_h / 2);
   assert(cur_screen == SCREEN_MODE_SELECT);
   printf("[OK] Mode button -> back to SCREEN_MODE_SELECT\n");

   /* Back key from the setup screen */
   input_handle_keypress(KEY_BACK);
   printf("[OK] KEY_BACK from mode_select handled without crash\n");

   /* Board-size toggle: tap "5x5", then Start, check the choice
      actually took effect end to end. draw_mode_select() (called by
      the KEY_BACK handler above) has already refreshed every
      hitbox global for this screen. */
   assert(selected_board_size == DEFAULT_BOARD_SIZE);
   input_handle_pointerup(board5_btn_x + board5_btn_w / 2,
                          board5_btn_y + board5_btn_h / 2);
   assert(selected_board_size == 5);
   printf("[OK] tap 5x5 toggle -> selected_board_size=%d\n", selected_board_size);

   /* Timer toggle: tap "1:30" too, in the same setup pass. */
   assert(selected_timer_seconds == TIMER_SECONDS);
   input_handle_pointerup(timer90_btn_x + timer90_btn_w / 2,
                          timer90_btn_y + timer90_btn_h / 2);
   assert(selected_timer_seconds == TIMER_SECONDS_SHORT);
   printf("[OK] tap 1:30 toggle -> selected_timer_seconds=%d\n",
          selected_timer_seconds);

   input_handle_pointerup(start_btn_x + start_btn_w / 2,
                          start_btn_y + start_btn_h / 2);
   assert(g.board_size == 5);
   assert(g.time_left == TIMER_SECONDS_SHORT);
   printf("[OK] new game after toggles -> g.board_size=%d, g.time_left=%d\n",
          g.board_size, g.time_left);

   fonts_free();
   printf("=== 4x4 FLOW TESTS PASSED ===\n\n");
   return 0;
}
