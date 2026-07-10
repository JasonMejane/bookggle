/* Game timer. Pure logic — no drawing here, see timer.h. */

#include "timer.h"
#include "game_state.h"

void game_timer_start(void) {
    SetHardTimer("BookggleTimer", on_timer_tick, 1000);
}

TimerTickResult game_timer_callback(int id) {
    (void)id;
    if (g.game_over) return TIMER_TICK_RUNNING;
    if (g.paused) return TIMER_TICK_RUNNING;

    g.time_left--;

    if (g.time_left <= 0) {
        g.time_left = 0;
        g.game_over = 1;

        if (g.mode == MODE_MULTI) {
            cur_screen = SCREEN_MULTI_END;
            return TIMER_TICK_ENDED_MULTI;
        }
        return TIMER_TICK_ENDED_SOLO;
    }

    return TIMER_TICK_RUNNING;
}
