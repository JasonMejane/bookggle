/* Ruleset selection. See ruleset.h. */

#include "ruleset.h"
#include "i18n.h"
#include <string.h>

extern const GameRuleset RULESET_EN_4X4;
extern const GameRuleset RULESET_EN_5X5;
extern const GameRuleset RULESET_FR_4X4;
extern const GameRuleset RULESET_FR_5X5;

static const GameRuleset *g_active_ruleset = NULL;

void ruleset_select(int board_size) {
    const char *lang = i18n_str(STR_LANG_CODE);

    if (strcmp(lang, "fr") == 0) {
        g_active_ruleset = (board_size == 5) ? &RULESET_FR_5X5 : &RULESET_FR_4X4;
        return;
    }

    /* English (and the fallback for any unrecognized language code,
       consistent with i18n's own English fallback). */
    g_active_ruleset = (board_size == 5) ? &RULESET_EN_5X5 : &RULESET_EN_4X4;
}

const GameRuleset *ruleset_active(void) {
    return (g_active_ruleset != NULL) ? g_active_ruleset : &RULESET_EN_4X4;
}
