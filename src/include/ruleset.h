/* Per-language, per-board-size Boggle rules: dice letters, Q/Qu
   digraph, minimum word length. Language comes from the device; board
   size is a separate player choice (see ruleset_select()). Rationale:
   docs/i18n_plan_tier2.md, docs/board_size_5x5_plan.md. */

#ifndef RULESET_H
#define RULESET_H

#include "game_state.h" /* MAX_DICE_COUNT */
#include "dice.h"       /* DICE_FACES */

typedef struct
{
   const char lang_code[8];
   int board_size;                              /* 4 or 5 */
   int dice_count;                              /* board_size^2, <= MAX_DICE_COUNT */
   const char dice[MAX_DICE_COUNT][DICE_FACES]; /* only [0..dice_count) used */
   int has_qu_digraph;
   int min_word_len;
} GameRuleset;

/* Picks the active ruleset from the device language
   (i18n_str(STR_LANG_CODE)) and board size. Call from EVT_INIT after
   i18n_init(), and again whenever the board-size toggle changes.
   Unrecognized languages fall back to English. */
void ruleset_select(int board_size);

/* Currently active ruleset. Never NULL -- defaults to the English
   4x4 ruleset if ruleset_select() hasn't been called yet. */
const GameRuleset *ruleset_active(void);

#endif /* RULESET_H */
