/* Checks game_start() really uses dice_roll_grid() under real
   conditions (not just dice.c in isolation).
   Run: make test-integration (from tests/), or see tests/README.md. */

#include "game_state.h"
#include "game_logic.h"
#include "ui_fonts.h"
#include "timer.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

/* timer.c needs this symbol to link; real version lives in src/main.c. */
void on_timer_tick(int id) {
    (void)game_timer_callback(id);
}

int main(void) {
    srand((unsigned)time(NULL));
    layout_compute();

    int total_runs = 200;
    for (int run = 0; run < total_runs; run++) {
        game_start(MODE_SOLO);

        int q = 0, z = 0;
        for (int r = 0; r < g.board_size; r++) {
            for (int c = 0; c < g.board_size; c++) {
                char ch = g.grid[r][c];
                assert(ch >= 'A' && ch <= 'Z');
                if (ch == 'Q') q++;
                if (ch == 'Z') z++;
            }
        }
        assert(q <= 1);
        assert(z <= 1);
    }

    printf("[OK] %d calls to game_start(MODE_SOLO): all grids valid "
           "(A-Z letters, <=1 Q, <=1 Z) via dice_roll_grid.\n", total_runs);
    printf("\n=== INTEGRATION TEST PASSED (1/1) ===\n");
    return 0;
}
