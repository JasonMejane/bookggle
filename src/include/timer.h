/* Game timer. Pure logic only — no drawing, no screen_* dependency. */

#ifndef TIMER_H
#define TIMER_H

typedef enum {
    TIMER_TICK_RUNNING,
    TIMER_TICK_ENDED_SOLO,
    TIMER_TICK_ENDED_MULTI
} TimerTickResult;

/* Starts the 1-tick-per-second timer. */
void game_timer_start(void);

/* Ticks the timer, returns what happened. Draws nothing. No-op
   (returns TIMER_TICK_RUNNING without decrementing time_left) while
   g.paused, same as while g.game_over -- the tick still happens on
   schedule, it just doesn't count down. */
TimerTickResult game_timer_callback(int id);

/* SetHardTimer needs void(*)(int), incompatible with the return value
   above. on_timer_tick is the real registered callback; it calls
   game_timer_callback and picks the redraw. Defined in main.c. */
void on_timer_tick(int id);

#endif /* TIMER_H */
