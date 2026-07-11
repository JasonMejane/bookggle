/* Full 5x5 game cycle: game_start() under selected_board_size=5,
   touch/select/word-building using g.board_size math (not a stale
   4x4 assumption), reaching a cell that wouldn't exist on a 4x4
   board, and the min_word_len=4 Big Boggle rule actually being
   enforced (a 3-letter word that would be valid at 4x4 must be
   rejected at 5x5).
   Run: make test-5x5 (from tests/), or see README.md. */

#include "game_state.h"
#include "ui_fonts.h"
#include "game_logic.h"
#include "timer.h"
#include "ruleset.h"
#include "i18n.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

/* timer.c needs this symbol to link; real version (with drawing)
   lives in src/main.c, not built into this test binary. */
void on_timer_tick(int id)
{
   (void)game_timer_callback(id);
}

int main(void)
{
   fonts_load();
   layout_compute();
   i18n_init();

   selected_board_size = 5;
   game_start(MODE_SOLO);
   assert(g.board_size == 5);
   assert(ruleset_active()->board_size == 5);
   assert(ruleset_active()->min_word_len == 4);
   printf("[OK] game_start() with selected_board_size=5 -> "
          "g.board_size=5, ruleset min_word_len=4\n");

   /* Deterministic grid: A,B,C,D,E / F,G,H,I,J / ... row-major,
      with Q forced at (0,0) for the digraph check. Row 4, col 4
      (bottom-right corner) wouldn't exist on a 4x4 board -- reaching
      it exercises the actual grown board_size, not just "still
      works at the old size". */
   for (int r = 0; r < 5; r++)
      for (int c = 0; c < 5; c++)
         g.grid[r][c] = (char)('A' + r * 5 + c);
   g.grid[0][0] = 'Q';

   int cx[5], cy[5];
   for (int i = 0; i < 5; i++)
   {
      cx[i] = GRID_X + i * CELL_SIZE + CELL_SIZE / 2;
      cy[i] = GRID_Y + i * CELL_SIZE + CELL_SIZE / 2;
   }

   /* Touch the bottom-right corner (row 4, col 4) directly -- only
      reachable if cell_from_coords()'s bounds check actually uses
      g.board_size=5, not a stale 4. */
   handle_cell_touch(cx[4], cy[4]);
   assert(g.sel_count == 1);
   printf("[OK] touch at row 4, col 4 (5x5-only cell) selected "
          "successfully, current_word='%s'\n",
          g.current_word);
   game_reset_selection();

   /* Build "QUB" (Q-digraph + adjacent B): 3 letters -- valid at
      4x4's min_word_len=3, but must be REJECTED here since Big
      Boggle requires 4+. This is the key new rule to verify. */
   handle_cell_touch(cx[0], cy[0]); /* (0,0) = Q -> "QU" */
   handle_cell_touch(cx[1], cy[0]); /* (0,1) = B, adjacent   */
   assert(strcmp(g.current_word, "QUB") == 0);

   int word_count_before = g.word_count;
   validate_word();
   assert(g.word_count == word_count_before); /* rejected, too short */
   assert(g.sel_count == 0);                  /* selection still resets */
   printf("[OK] 3-letter word 'QUB' rejected at 5x5 "
          "(min_word_len=4, word_count unchanged=%d)\n",
          g.word_count);

   /* Build a 4-letter word from the same start: Q,U,B,G -- (0,0),
      (0,1), (1,0)=G. (0,0)=Q, (0,1)=B, (1,1)=G (adjacent to both). */
   handle_cell_touch(cx[0], cy[0]); /* (0,0) = Q -> "QU" */
   handle_cell_touch(cx[1], cy[0]); /* (0,1) = B         */
   handle_cell_touch(cx[1], cy[1]); /* (1,1) = G         */
   assert(strcmp(g.current_word, "QUBG") == 0);

   word_count_before = g.word_count;
   int score_before = g.score;
   validate_word();
   assert(g.word_count == word_count_before + 1);
   assert(g.score == score_before + score_for_word(4));
   printf("[OK] 4-letter word 'QUBG' accepted at 5x5 "
          "(word_count=%d, score=%d)\n",
          g.word_count, g.score);

   fonts_free();
   printf("=== 5x5 FLOW TESTS PASSED ===\n\n");
   return 0;
}
