/* Game timer. Pure logic only — no drawing, no screen_* dependency. */

#ifndef TIMER_H
#define TIMER_H

typedef enum
{
   TIMER_TICK_RUNNING,
   TIMER_TICK_ENDED_SOLO,
   TIMER_TICK_ENDED_MULTI
} TimerTickResult;

/* Arms the timer for the next 1-second tick. SetHardTimer is one-shot,
   so this is called both to start the timer and to re-arm it after
   every tick that keeps the game running. */
void game_timer_start(void);

/* Ticks the timer, returns what happened. Draws nothing. Re-arms the
   one-shot timer while the game keeps running (via game_timer_start),
   and stops re-arming on the tick that ends the game. No-op on
   time_left (returns TIMER_TICK_RUNNING without decrementing) while
   g.paused, same as while g.game_over -- the tick still happens on
   schedule, it just doesn't count down. */
TimerTickResult game_timer_callback(int id);

/* SetHardTimer needs void(*)(int), incompatible with the return value
   above. on_timer_tick is the real registered callback; it calls
   game_timer_callback and picks the redraw. Defined in main.c. */
void on_timer_tick(int id);

#endif /* TIMER_H */
