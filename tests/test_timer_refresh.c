#include "game_state.h"
#include "timer.h"
#include "screen_game.h"
#include "ui_fonts.h"
#include "stub_hooks.h"
#include <assert.h>
#include <stdio.h>

void on_timer_tick(int id) {
    (void)id;
    switch (game_timer_callback(id)) {
    case TIMER_TICK_RUNNING:
        draw_game();
        break;
    case TIMER_TICK_ENDED_SOLO:
    case TIMER_TICK_ENDED_MULTI:
        break;
    }
}

int main(void) {
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
    printf("timer refresh test passed\n");
    return 0;
}
