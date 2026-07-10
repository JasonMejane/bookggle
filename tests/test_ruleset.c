/* Checks ruleset_select(board_size)/ruleset_active(): picks the right
   ruleset for each of the 4 bundled (language, board_size) pairs,
   falls back to English for an unrecognized device language (at
   either board size), and never returns NULL (even before
   ruleset_select() is ever called).
   Run: make test-ruleset (from tests/), or see tests/README.md. */

#include "game_state.h"
#include "ruleset.h"
#include "i18n.h"
#include "stub_hooks.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

int main(void) {
    i18n_init();

    /* Never NULL, even before ruleset_select() has run -- defaults
       to English 4x4. */
    const GameRuleset *before = ruleset_active();
    assert(before != NULL);
    assert(strcmp(before->lang_code, "en") == 0);
    assert(before->board_size == 4);
    printf("[OK] ruleset_active() defaults to English 4x4 before "
           "ruleset_select() is called\n");

    /* en + 4x4 -> RULESET_EN_4X4 */
    stub_set_current_lang("en");
    ruleset_select(4);
    const GameRuleset *en4 = ruleset_active();
    assert(strcmp(en4->lang_code, "en") == 0);
    assert(en4->board_size == 4);
    assert(en4->dice_count == 16);
    assert(en4->min_word_len == 3);
    printf("[OK] en + board_size=4 -> English 4x4 (16 dice, min_word_len=3)\n");

    /* en + 5x5 -> RULESET_EN_5X5 */
    ruleset_select(5);
    const GameRuleset *en5 = ruleset_active();
    assert(strcmp(en5->lang_code, "en") == 0);
    assert(en5->board_size == 5);
    assert(en5->dice_count == 25);
    assert(en5->min_word_len == 4);
    printf("[OK] en + board_size=5 -> English 5x5 (25 dice, min_word_len=4)\n");

    /* fr + 4x4 -> RULESET_FR_4X4 */
    stub_set_current_lang("fr");
    ruleset_select(4);
    const GameRuleset *fr4 = ruleset_active();
    assert(strcmp(fr4->lang_code, "fr") == 0);
    assert(fr4->board_size == 4);
    assert(fr4->dice_count == 16);
    assert(fr4->min_word_len == 3);
    printf("[OK] fr + board_size=4 -> French 4x4 (16 dice, min_word_len=3)\n");

    /* fr + 5x5 -> RULESET_FR_5X5 */
    ruleset_select(5);
    const GameRuleset *fr5 = ruleset_active();
    assert(strcmp(fr5->lang_code, "fr") == 0);
    assert(fr5->board_size == 5);
    assert(fr5->dice_count == 25);
    assert(fr5->min_word_len == 4);
    printf("[OK] fr + board_size=5 -> French 5x5 (25 dice, min_word_len=4)\n");

    /* Unknown device language -> falls back to English, not a crash
       or a NULL ruleset. Board size is still honored since English
       has both sizes. */
    stub_set_current_lang("zz");
    ruleset_select(4);
    const GameRuleset *fallback4 = ruleset_active();
    assert(fallback4 != NULL);
    assert(strcmp(fallback4->lang_code, "en") == 0);
    assert(fallback4->board_size == 4);

    ruleset_select(5);
    const GameRuleset *fallback5 = ruleset_active();
    assert(strcmp(fallback5->lang_code, "en") == 0);
    assert(fallback5->board_size == 5);
    printf("[OK] unknown device language falls back to English at "
           "both board sizes\n");

    /* Sanity on the bundled data itself: all 4 rulesets use the Qu
       digraph; min_word_len is 3 at 4x4 and 4 at 5x5 (Big Boggle /
       Big B@ggle rule), confirmed via official rule sheets -- see
       docs/board_size_5x5_plan.md. */
    assert(en4->has_qu_digraph == 1);
    assert(en5->has_qu_digraph == 1);
    assert(fr4->has_qu_digraph == 1);
    assert(fr5->has_qu_digraph == 1);
    printf("[OK] all 4 bundled rulesets use the Qu digraph\n");

    printf("\n=== RULESET TESTS PASSED ===\n");
    return 0;
}
