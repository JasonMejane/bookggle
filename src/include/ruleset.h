/* Per-language, per-board-size Boggle rules: dice letters, Q/Qu
   digraph, minimum word length. See docs/i18n_plan_tier2.md and
   docs/board_size_5x5_plan.md for the design rationale.

   Language is picked from the device (i18n's STR_LANG_CODE). Board
   size is a separate, player-facing choice (a French device doesn't
   imply a French player wants a bigger or smaller grid) -- see
   ruleset_select(). */

#ifndef RULESET_H
#define RULESET_H

#include "game_state.h" /* MAX_DICE_COUNT */
#include "dice.h"        /* DICE_FACES */

typedef struct {
    const char lang_code[8];
    int        board_size;                        /* 4 or 5 */
    int        dice_count;                         /* board_size^2, <= MAX_DICE_COUNT */
    const char dice[MAX_DICE_COUNT][DICE_FACES];    /* only [0..dice_count) used */
    int        has_qu_digraph;
    int        min_word_len;
} GameRuleset;

/* Picks the active ruleset from the current device language (via
   i18n_str(STR_LANG_CODE)) and the requested board size. Call from
   EVT_INIT (after i18n_init()) with the default board size, and
   again whenever the player changes the board-size toggle on the
   mode-select screen.

   Four rulesets are bundled: English and French, each at 4x4 and
   5x5. If a future language/board-size combination isn't bundled,
   the convention is to fall back to the SAME language at its next
   available board size (see ruleset.c's history for a worked
   example, from when French 5x5 wasn't sourced yet) rather than
   silently switching language just to honor the size request. */
void ruleset_select(int board_size);

/* Currently active ruleset. Never NULL -- defaults to the English
   4x4 ruleset if ruleset_select() hasn't been called yet. */
const GameRuleset *ruleset_active(void);

#endif /* RULESET_H */
