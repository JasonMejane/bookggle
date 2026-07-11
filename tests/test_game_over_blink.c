#include "game_state.h"
#include "screen_game.h"
#include "timer.h"
#include "ui_fonts.h"
#include "stub_hooks.h"
#include <assert.h>
#include <stdio.h>

/* timer.c references on_timer_tick; this suite never fires the timer. */
void on_timer_tick(int id) { (void)id; }

int main(void)
{
    fonts_load();
    layout_compute();
    layout_compute_grid(4);

    g.board_size = 4;
    g.mode = MODE_SOLO;
    g.game_over = 1;
    g.paused = 0;
    g.sel_count = 0;

    stub_reset_update_counters();
    grid_game_over_blink();

    /* Each blink repaints the grid twice (normal then dark), so the board
       flashes GAME_OVER_BLINKS times over the grid region and settles
       dark. Uses partial updates only -- no full-screen refresh. */
    assert(stub_partial_update_call_count() == GAME_OVER_BLINKS * 2);
    assert(stub_full_update_call_count() == 0);

    printf("[OK] game-over blink flashes grid %d times via partial updates\n",
           GAME_OVER_BLINKS);
    printf("=== GAME OVER BLINK TESTS PASSED ===\n\n");
    return 0;
}
