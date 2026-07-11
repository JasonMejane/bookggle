/* Touch and key event routing. */

#include "input.h"
#include "game_state.h"
#include "game_logic.h"
#include "screen_mode_select.h"
#include "screen_game.h"
#include "screen_multi_end.h"

static void handle_tap_mode_select(int tx, int ty)
{
    if (tx >= mode_solo_btn_x && tx <= mode_solo_btn_x + mode_solo_btn_w &&
        ty >= mode_solo_btn_y && ty <= mode_solo_btn_y + mode_solo_btn_h)
    {
        selected_mode = MODE_SOLO;
        draw_mode_select();
        return;
    }

    if (tx >= mode_multi_btn_x && tx <= mode_multi_btn_x + mode_multi_btn_w &&
        ty >= mode_multi_btn_y && ty <= mode_multi_btn_y + mode_multi_btn_h)
    {
        selected_mode = MODE_MULTI;
        draw_mode_select();
        return;
    }

    if (tx >= board4_btn_x && tx <= board4_btn_x + board4_btn_w &&
        ty >= board4_btn_y && ty <= board4_btn_y + board4_btn_h)
    {
        selected_board_size = 4;
        draw_mode_select();
        return;
    }

    if (tx >= board5_btn_x && tx <= board5_btn_x + board5_btn_w &&
        ty >= board5_btn_y && ty <= board5_btn_y + board5_btn_h)
    {
        selected_board_size = 5;
        draw_mode_select();
        return;
    }

    if (tx >= timer90_btn_x && tx <= timer90_btn_x + timer90_btn_w &&
        ty >= timer90_btn_y && ty <= timer90_btn_y + timer90_btn_h)
    {
        selected_timer_seconds = TIMER_SECONDS_SHORT;
        draw_mode_select();
        return;
    }

    if (tx >= timer180_btn_x && tx <= timer180_btn_x + timer180_btn_w &&
        ty >= timer180_btn_y && ty <= timer180_btn_y + timer180_btn_h)
    {
        selected_timer_seconds = TIMER_SECONDS;
        draw_mode_select();
        return;
    }

    if (tx >= quit_btn_x && tx <= quit_btn_x + quit_btn_w &&
        ty >= quit_btn_y && ty <= quit_btn_y + quit_btn_h)
    {
        CloseApp();
        return;
    }

    if (tx >= start_btn_x && tx <= start_btn_x + start_btn_w &&
        ty >= start_btn_y && ty <= start_btn_y + start_btn_h)
    {
        game_start(selected_mode);
        draw_game();
    }
}

static void handle_tap_multi_end(int tx, int ty)
{
    if (tx >= newgame_btn_x && tx <= newgame_btn_x + newgame_btn_w &&
        ty >= newgame_btn_y && ty <= newgame_btn_y + newgame_btn_h)
    {
        game_start(MODE_MULTI);
        draw_game();
    }
    else if (tx >= changemode_btn_x && tx <= changemode_btn_x + changemode_btn_w &&
             ty >= changemode_btn_y && ty <= changemode_btn_y + changemode_btn_h)
    {
        cur_screen = SCREEN_MODE_SELECT;
        draw_mode_select();
    }
}

static void handle_tap_game(int tx, int ty)
{
    if (tx >= quit_btn_x && tx <= quit_btn_x + quit_btn_w &&
        ty >= quit_btn_y && ty <= quit_btn_y + quit_btn_h)
    {
        CloseApp();
        return;
    }

    if (tx >= mode_btn_x && tx <= mode_btn_x + mode_btn_w &&
        ty >= mode_btn_y && ty <= mode_btn_y + mode_btn_h)
    {
        game_cancel();
        cur_screen = SCREEN_MODE_SELECT;
        draw_mode_select();
        return;
    }

    if (!g.game_over &&
        tx >= pause_btn_x && tx <= pause_btn_x + pause_btn_w &&
        ty >= pause_btn_y && ty <= pause_btn_y + pause_btn_h)
    {
        g.paused = !g.paused;
        draw_game();
        return;
    }

    if (g.mode == MODE_SOLO && !g.game_over)
    {
        if (tx >= submit_btn_x && tx <= submit_btn_x + submit_btn_w &&
            ty >= submit_btn_y && ty <= submit_btn_y + submit_btn_h)
        {
            validate_word();
            draw_game();
            return;
        }

        if (tx >= clear_btn_x && tx <= clear_btn_x + clear_btn_w &&
            ty >= clear_btn_y && ty <= clear_btn_y + clear_btn_h)
        {
            game_reset_selection();
            draw_game();
            return;
        }
    }

    handle_cell_touch(tx, ty);
    draw_game();
}

void input_handle_pointerup(int x, int y)
{
    switch (cur_screen)
    {
    case SCREEN_MODE_SELECT:
        handle_tap_mode_select(x, y);
        break;

    case SCREEN_MULTI_END:
        handle_tap_multi_end(x, y);
        break;

    case SCREEN_GAME:
        handle_tap_game(x, y);
        break;
    }
}

void input_handle_keypress(int key)
{
    if (key != KEY_BACK)
        return;

    if (cur_screen == SCREEN_GAME || cur_screen == SCREEN_MULTI_END)
    {
        game_cancel();
        cur_screen = SCREEN_MODE_SELECT;
        draw_mode_select();
    }
    else
    {
        CloseApp();
    }
}
