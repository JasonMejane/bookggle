/* App entry point. Wires InkView events to game_logic, screen draws,
   and input. on_timer_tick bridges timer.c (pure logic) to drawing. */

#include "inkview.h"
#include "game_state.h"
#include "ui_fonts.h"
#include "timer.h"
#include "screen_mode_select.h"
#include "screen_game.h"
#include "screen_multi_end.h"
#include "input.h"
#include "i18n.h"
#include "ruleset.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void on_timer_tick(int id) {
    switch (game_timer_callback(id)) {

    case TIMER_TICK_RUNNING:
        /* Only repaint the top HUD strip where the timer text lives.
           This avoids a full-screen redraw while still forcing an
           update for the e-ink screen. */
        draw_hud();
        PartialUpdate(0, 0, SW, SH / 5);
        break;

    case TIMER_TICK_ENDED_MULTI:
        /* draw_multi_end() clears the screen itself. */
        draw_multi_end();
        FullUpdate();
        break;

    case TIMER_TICK_ENDED_SOLO: {
        char msg[96];
        const char *fmt = (g.score > 1)
            ? i18n_str(STR_SCORE_FINAL_PLURAL)
            : i18n_str(STR_SCORE_FINAL_SINGULAR);
        snprintf(msg, sizeof(msg), fmt, g.score);
        Message(ICON_INFORMATION, i18n_str(STR_GAME_OVER_TITLE), msg, 5000);
        break;
    }
    }
}

static int main_handler(int type, int par1, int par2) {

    switch (type) {

    case EVT_INIT:
        srand((unsigned int)time(NULL));
        fonts_load();
        layout_compute();
        i18n_init();
        ruleset_select(DEFAULT_BOARD_SIZE);
        cur_screen = SCREEN_MODE_SELECT;
        break;

    case EVT_SHOW:
        switch (cur_screen) {
        case SCREEN_MODE_SELECT: draw_mode_select(); break;
        case SCREEN_GAME:        draw_game();        break;
        case SCREEN_MULTI_END:   draw_multi_end(); FullUpdate(); break;
        }
        break;

    case EVT_POINTERUP:
        input_handle_pointerup(par1, par2);
        break;

    case EVT_KEYPRESS:
        input_handle_keypress(par1);
        break;

    case EVT_EXIT:
        fonts_free();
        break;

    default:
        break;
    }
    return 0;
}

int main(int argc, char **argv) {
    (void)argc; (void)argv;
    InkViewMain(main_handler);
    return 0;
}
