/* English 5x5 "Big Boggle" ruleset (25-dice set). Dice letters
   researched from a physically-verified transcription: a contributor
   on Wikipedia's Talk:Boggle page checked the commonly-circulated
   list against an owned 1979 Big Boggle set and published 4
   corrections (the uncorrected list still circulates on
   boardgamegeek.com and dcode.fr). The corrected values match three
   independent source-code repositories exactly (ioanbsu/coursera,
   megulus/boggle_gist, Princeton's algs4 course testing
   BoggleBoard.java). See docs/board_size_5x5_plan.md.

   Note: dice #16 and #17 are both DHLNOR -- an intentional duplicate
   pair confirmed by the physical-verification source, not a
   transcription bug. Die #9 (BJKQXZ) carries both Q and Z on the
   same physical die -- also confirmed, not a bug.

   min_word_len = 4 (not 3): confirmed by multiple official rule
   sheets (UltraBoardGames, House of Games, Winning Moves product
   listings) and by baggle.org's own Big B@ggle rules text. Scoring
   table is unchanged from the 4x4 game (score_for_word() needs no
   change) -- confirmed via a quoted official Big Boggle point scheme
   (davidevans.blog, 2009) matching 1/1/2/3/5/11 exactly. */

#include "ruleset.h"
#include "game_state.h" /* MAX_DICE_COUNT */

const GameRuleset RULESET_EN_5X5 = {
    .lang_code = "en",
    .board_size = 5,
    .dice_count = 25,
    .dice = {
        {'A','A','A','F','R','S'},  /*  1: AAAFRS */
        {'A','A','E','E','E','E'},  /*  2: AAEEEE */
        {'A','A','F','I','R','S'},  /*  3: AAFIRS */
        {'A','D','E','N','N','N'},  /*  4: ADENNN */
        {'A','E','E','E','E','M'},  /*  5: AEEEEM */
        {'A','E','E','G','M','U'},  /*  6: AEEGMU */
        {'A','E','G','M','N','N'},  /*  7: AEGMNN */
        {'A','F','I','R','S','Y'},  /*  8: AFIRSY */
        {'B','J','K','Q','X','Z'},  /*  9: BJKQXZ */
        {'C','C','E','N','S','T'},  /* 10: CCENST */
        {'C','E','I','I','L','T'},  /* 11: CEIILT */
        {'C','E','I','L','P','T'},  /* 12: CEILPT */
        {'C','E','I','P','S','T'},  /* 13: CEIPST */
        {'D','D','H','N','O','T'},  /* 14: DDHNOT */
        {'D','H','H','L','O','R'},  /* 15: DHHLOR */
        {'D','H','L','N','O','R'},  /* 16: DHLNOR */
        {'D','H','L','N','O','R'},  /* 17: DHLNOR (duplicate, verified) */
        {'E','I','I','I','T','T'},  /* 18: EIIITT */
        {'E','M','O','T','T','T'},  /* 19: EMOTTT */
        {'E','N','S','S','S','U'},  /* 20: ENSSSU */
        {'F','I','P','R','S','Y'},  /* 21: FIPRSY */
        {'G','O','R','R','V','W'},  /* 22: GORRVW */
        {'I','P','R','R','R','Y'},  /* 23: IPRRRY */
        {'N','O','O','T','U','W'},  /* 24: NOOTUW */
        {'O','O','O','T','T','U'},  /* 25: OOOTTU */
    },
    .has_qu_digraph = 1,
    .min_word_len = 4,
};
