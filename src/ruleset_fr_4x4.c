/* French 4x4 Boggle ruleset. Dice source: http://baggle.org/regles.php

   See ruleset_fr_5x5.c for the Big B@ggle (25-dice) French ruleset. */

#include "ruleset.h"
#include "game_state.h" /* MIN_WORD_LEN */

const GameRuleset RULESET_FR_4X4 = {
    .lang_code = "fr",
    .board_size = 4,
    .dice_count = 16,
    .dice = {
        {'E', 'T', 'U', 'K', 'N', 'O'},
        {'E', 'V', 'G', 'T', 'I', 'N'},
        {'D', 'E', 'C', 'A', 'M', 'P'},
        {'I', 'E', 'L', 'R', 'U', 'W'},
        {'E', 'H', 'I', 'F', 'S', 'E'},
        {'R', 'E', 'C', 'A', 'L', 'S'},
        {'E', 'N', 'T', 'D', 'O', 'S'},
        {'O', 'F', 'X', 'R', 'I', 'A'},
        {'N', 'A', 'V', 'E', 'D', 'Z'},
        {'E', 'I', 'O', 'A', 'T', 'A'},
        {'G', 'L', 'E', 'N', 'Y', 'U'},
        {'B', 'M', 'A', 'Q', 'J', 'O'},
        {'T', 'L', 'I', 'B', 'R', 'A'},
        {'S', 'P', 'U', 'L', 'T', 'E'},
        {'A', 'I', 'M', 'S', 'O', 'R'},
        {'E', 'N', 'H', 'R', 'I', 'S'},
        /* [16..24] unused at 4x4, zero-filled */
    },
    .has_qu_digraph = 1,
    .min_word_len = MIN_WORD_LEN,
};
