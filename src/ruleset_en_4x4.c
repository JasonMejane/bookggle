/* English 4x4 Boggle ruleset ("New Boggle" 16-dice set). Dice letters
   researched from https://boardgames.stackexchange.com/q/48151 and
   cross-checked against several independently published transcriptions
   (Stanford CS106B assignment starter code, multiple Boggle solver
   repos, "A Programmer's Analysis of Boggle" by Robert Gamble) that
   all converge on the same 16 dice. The Q die is conventionally read
   as "Qu", same convention as the French set. */

#include "ruleset.h"
#include "game_state.h" /* MIN_WORD_LEN */

const GameRuleset RULESET_EN_4X4 = {
    .lang_code = "en",
    .board_size = 4,
    .dice_count = 16,
    .dice = {
        {'A', 'A', 'E', 'E', 'G', 'N'},
        {'A', 'B', 'B', 'J', 'O', 'O'},
        {'A', 'C', 'H', 'O', 'P', 'S'},
        {'A', 'F', 'F', 'K', 'P', 'S'},
        {'A', 'O', 'O', 'T', 'T', 'W'},
        {'C', 'I', 'M', 'O', 'T', 'U'},
        {'D', 'E', 'I', 'L', 'R', 'X'},
        {'D', 'E', 'L', 'R', 'V', 'Y'},
        {'D', 'I', 'S', 'T', 'T', 'Y'},
        {'E', 'E', 'G', 'H', 'N', 'W'},
        {'E', 'E', 'I', 'N', 'S', 'U'},
        {'E', 'H', 'R', 'T', 'V', 'W'},
        {'E', 'I', 'O', 'S', 'S', 'T'},
        {'E', 'L', 'R', 'T', 'T', 'Y'},
        {'H', 'I', 'M', 'N', 'Q', 'U'},
        {'H', 'L', 'N', 'N', 'R', 'Z'},
        /* [16..24] unused at 4x4, zero-filled */
    },
    .has_qu_digraph = 1,
    .min_word_len = MIN_WORD_LEN,
};
