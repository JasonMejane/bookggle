#include "game_state.h"
#include "game_logic.h"
#include "timer.h"
#include "screen_game.h"
#include "ui_fonts.h"
#include "stub_hooks.h"
#include <assert.h>
#include <stdio.h>

void on_timer_tick(int id)
{
    (void)id;
    switch (game_timer_callback(id))
    {
    case TIMER_TICK_RUNNING:
        draw_game();
        break;
    case TIMER_TICK_ENDED_SOLO:
    case TIMER_TICK_ENDED_MULTI:
        break;
    }
}

int main(void)
{
    fonts_load();
    layout_compute();

    g.time_left = 3;
    g.game_over = 0;
    g.paused = 0;
    g.mode = MODE_SOLO;
    cur_screen = SCREEN_GAME;

    stub_reset_update_counters();
    on_timer_tick(0);

    assert(g.time_left == 2);
    assert(stub_full_update_call_count() == 1);

    /* A running tick must re-arm the one-shot device timer, otherwise it
       fires only once and time freezes at 2:59 on the device. */
    assert(stub_hard_timer_call_count() == 1);

    /* The tick that ends the game must NOT re-arm the timer. */
    g.time_left = 1;
    g.game_over = 0;
    stub_reset_update_counters();
    on_timer_tick(0);
    assert(g.game_over == 1);
    assert(stub_hard_timer_call_count() == 0);

    /* Navigating away mid-game (e.g. the Mode button) must cancel the
       game like a timeout would: the already-armed tick that follows
       must not re-arm the timer, so it can't keep ticking (and
       redrawing the HUD) behind the mode-select screen. */
    g.time_left = 42;
    g.game_over = 0;
    cur_screen = SCREEN_GAME;
    game_cancel();
    cur_screen = SCREEN_MODE_SELECT;
    stub_reset_update_counters();
    on_timer_tick(0);
    assert(g.game_over == 1);
    assert(g.time_left == 42); /* not decremented once canceled */
    assert(stub_hard_timer_call_count() == 0);

    printf("=== TIMER REFRESH TESTS PASSED ===\n\n");
    return 0;
}
