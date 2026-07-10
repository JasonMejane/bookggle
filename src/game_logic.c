/* Game rules: grid, touch selection, words, score. */

#include "game_logic.h"
#include "timer.h"
#include "dice.h"
#include "ruleset.h"
#include "i18n.h"
#include "ui_fonts.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void game_reset_selection(void) {
    g.sel_count = 0;
    g.current_word[0] = '\0';
    memset(g.selected, -1, sizeof(g.selected));
}

void game_start(GameMode mode) {
    ruleset_select(selected_board_size);
    dice_roll_grid();                       /* sets g.board_size */
    layout_compute_grid(g.board_size);

    g.word_count = 0;
    g.score      = 0;
    g.time_left  = selected_timer_seconds;
    g.game_over  = 0;
    g.paused     = 0;
    g.mode       = mode;
    game_reset_selection();
    cur_screen = SCREEN_GAME;

    game_timer_start();
}

const char *cell_label(int r, int c) {
    static char buf[4];
    if (g.grid[r][c] == 'Q' && ruleset_active()->has_qu_digraph) {
        buf[0] = 'Q'; buf[1] = 'u'; buf[2] = '\0';
    } else {
        buf[0] = g.grid[r][c]; buf[1] = '\0';
    }
    return buf;
}

void cell_contrib(int r, int c, char *out, int maxout) {
    if (g.grid[r][c] == 'Q' && ruleset_active()->has_qu_digraph)
        snprintf(out, maxout, "QU");
    else
        snprintf(out, maxout, "%c", g.grid[r][c]);
}

int cell_from_coords(int x, int y, int *row, int *col) {
    if (x < GRID_X || y < GRID_Y) return 0;
    int c = (x - GRID_X) / CELL_SIZE;
    int r = (y - GRID_Y) / CELL_SIZE;
    if (r < 0 || r >= g.board_size || c < 0 || c >= g.board_size) return 0;
    *row = r; *col = c;
    return 1;
}

int is_adjacent(int i1, int i2) {
    int r1 = i1 / g.board_size, c1 = i1 % g.board_size;
    int r2 = i2 / g.board_size, c2 = i2 % g.board_size;
    return (i1 != i2) && (abs(r1 - r2) <= 1) && (abs(c1 - c2) <= 1);
}

int already_selected(int idx) {
    for (int i = 0; i < g.sel_count; i++)
        if (g.selected[i] == idx) return 1;
    return 0;
}

static void rebuild_word(void) {
    g.current_word[0] = '\0';
    for (int i = 0; i < g.sel_count; i++) {
        int ri = g.selected[i] / g.board_size;
        int ci = g.selected[i] % g.board_size;
        char contrib[4];
        cell_contrib(ri, ci, contrib, sizeof(contrib));
        strncat(g.current_word, contrib,
                MAX_WORD_LEN - strlen(g.current_word) - 1);
    }
}

void handle_cell_touch(int x, int y) {
    if (g.game_over || cur_screen != SCREEN_GAME) return;
    if (g.mode == MODE_MULTI) return;
    if (g.paused) return;

    int row, col;
    if (!cell_from_coords(x, y, &row, &col)) return;
    int idx = row * g.board_size + col;

    if (g.sel_count > 0 && g.selected[g.sel_count - 1] == idx) {
        g.sel_count--;
        rebuild_word();
        return;
    }

    if (g.sel_count > 0 && !is_adjacent(g.selected[g.sel_count - 1], idx))
        return;
    if (already_selected(idx)) return;

    if (g.sel_count < g.board_size * g.board_size) {
        g.selected[g.sel_count++] = idx;
        rebuild_word();
    }
}

int word_already_found(const char *w) {
    for (int i = 0; i < g.word_count; i++)
        if (strcmp(g.found_words[i], w) == 0) return 1;
    return 0;
}

int word_is_valid(const char *w) {
    return (int)strlen(w) >= ruleset_active()->min_word_len;
}

int score_for_word(int len) {
    if (len <= 4) return 1;
    if (len == 5) return 2;
    if (len == 6) return 3;
    if (len == 7) return 5;
    return 11;
}

void validate_word(void) {
    if (g.game_over || g.mode == MODE_MULTI) return;
    int len = (int)strlen(g.current_word);

    if (len < ruleset_active()->min_word_len) {
        char body[64];
        snprintf(body, sizeof(body), i18n_str(STR_TOO_SHORT_BODY),
                 ruleset_active()->min_word_len);
        Message(ICON_WARNING, i18n_str(STR_TOO_SHORT_TITLE), body, 1500);
    } else if (word_already_found(g.current_word)) {
        Message(ICON_WARNING, i18n_str(STR_ALREADY_FOUND_TITLE),
                i18n_str(STR_ALREADY_FOUND_BODY), 1500);
    } else if (!word_is_valid(g.current_word)) {
        Message(ICON_WARNING, i18n_str(STR_INVALID_TITLE),
                i18n_str(STR_INVALID_BODY), 1500);
    } else {
        if (g.word_count < MAX_WORDS) {
            snprintf(g.found_words[g.word_count], MAX_WORD_LEN, "%s",
                     g.current_word);
            g.word_count++;
        }
        g.score += score_for_word(len);
    }
    game_reset_selection();
}
