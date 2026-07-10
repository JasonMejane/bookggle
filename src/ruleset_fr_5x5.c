/* French 5x5 "Big B@ggle" ruleset (25-dice set). Dice letters
   provided directly (not independently sourced/cited like the other
   three bundled rulesets — see docs/board_size_5x5_plan.md, open
   question 1, for why this was previously unsourced).

   Note: unlike the other three bundled rulesets, this table has TWO
   dice carrying Q (die 9: NAATEQ, die 19: RZNNTQ) rather than one.
   Z appears once, on the same die as one of the Qs (die 19). This is
   a genuine characteristic of this specific table, not a
   transcription error — tests/test_dice.c's Q/Z uniqueness check is
   parameterized per ruleset rather than assuming "at most one of
   each" universally, to account for it.

   min_word_len = 4, has_qu_digraph = 1: consistent with the other
   Big Boggle/Big B@ggle rulesets already bundled (English 5x5,
   confirmed via multiple official rule sheets; baggle.org's own Big
   B@ggle rules text for French, which specifies the same 4-letter
   minimum). */

#include "ruleset.h"
#include "game_state.h" /* MAX_DICE_COUNT */

const GameRuleset RULESET_FR_5X5 = {
    .lang_code = "fr",
    .board_size = 5,
    .dice_count = 25,
    .dice = {
        {'M', 'D', 'N', 'S', 'N', 'H'}, /*  1: MDNSNH */
        {'G', 'F', 'S', 'T', 'E', 'Y'}, /*  2: GFSTEY */
        {'L', 'M', 'T', 'R', 'X', 'S'}, /*  3: LMTRXS */
        {'T', 'T', 'R', 'S', 'C', 'H'}, /*  4: TTRSCH */
        {'B', 'M', 'L', 'N', 'D', 'L'}, /*  5: BMLNDL */
        {'T', 'M', 'R', 'D', 'B', 'T'}, /*  6: TMRDBT */
        {'E', 'I', 'U', 'E', 'A', 'O'}, /*  7: EIUEAO */
        {'R', 'L', 'X', 'S', 'S', 'B'}, /*  8: RLXSSB */
        {'N', 'A', 'A', 'T', 'E', 'Q'}, /*  9: NAATEQ */
        {'T', 'C', 'J', 'F', 'S', 'H'}, /* 10: TCJFSH */
        {'I', 'E', 'E', 'A', 'O', 'A'}, /* 11: IEEAOA */
        {'N', 'D', 'H', 'S', 'N', 'M'}, /* 12: NDHSNM */
        {'I', 'A', 'A', 'I', 'E', 'O'}, /* 13: IAAIEO */
        {'O', 'E', 'U', 'E', 'I', 'A'}, /* 14: OEUEIA */
        {'L', 'C', 'P', 'R', 'J', 'S'}, /* 15: LCPRJS */
        {'D', 'S', 'T', 'L', 'S', 'M'}, /* 16: DSTLSM */
        {'N', 'K', 'L', 'P', 'F', 'N'}, /* 17: NKLPFN */
        {'D', 'W', 'R', 'N', 'L', 'P'}, /* 18: DWRNLP */
        {'R', 'Z', 'N', 'N', 'T', 'Q'}, /* 19: RZNNTQ */
        {'R', 'G', 'L', 'R', 'V', 'F'}, /* 20: RGLRVF */
        {'R', 'V', 'C', 'G', 'R', 'T'}, /* 21: RVCGRT */
        {'I', 'I', 'O', 'E', 'A', 'E'}, /* 22: IIOEAE */
        {'E', 'U', 'I', 'A', 'E', 'O'}, /* 23: EUIAEO */
        {'U', 'I', 'A', 'E', 'O', 'A'}, /* 24: UIAEOA */
        {'N', 'S', 'E', 'V', 'A', 'E'}, /* 25: NSEVAE */
    },
    .has_qu_digraph = 1,
    .min_word_len = 4,
};
