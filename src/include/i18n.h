/* i18n: string keys, translation lookup, button-fit font sizing.
   See docs/i18n_plan_tier1.md for the design rationale.

   Assumes GetCurrentLangText() resolves a label against the device's
   system language with English fallback (per the SDK header's own
   note) -- not verified on hardware, but i18n_str() has a defensive
   compiled-in English fallback regardless. */

#ifndef I18N_H
#define I18N_H

#include "inkview.h"

typedef enum
{
   STR_CHOOSE_MODE,
   STR_BTN_SOLO,
   STR_SOLO_HINT,
   STR_BTN_MULTIPLAYER,
   STR_MULTI_HINT,
   STR_LABEL_BOARD_SIZE,
   STR_LABEL_TIMER,
   STR_BTN_START,

   STR_HUD_TITLE_SOLO,
   STR_HUD_TITLE_MULTI,
   STR_SCORE_LABEL,
   STR_BTN_MODE,
   STR_BTN_QUIT,
   STR_BTN_PAUSE,
   STR_BTN_PLAY,
   STR_BTN_SUBMIT,
   STR_BTN_CLEAR,
   STR_WORDS_HEADER,
   STR_MULTI_INGAME_HINT,

   STR_TIMES_UP,
   STR_COMPARE_LISTS,
   STR_CANCEL_RULE,
   STR_BTN_NEW_GAME,
   STR_BTN_CHANGE_MODE,

   STR_GAME_OVER_TITLE,
   STR_SCORE_FINAL_SINGULAR,
   STR_SCORE_FINAL_PLURAL,

   STR_TOO_SHORT_TITLE,
   STR_TOO_SHORT_BODY,
   STR_ALREADY_FOUND_TITLE,
   STR_ALREADY_FOUND_BODY,
   STR_INVALID_TITLE,
   STR_INVALID_BODY,

   /* Not user-facing: each table's own "en"/"fr" code, resolved via
      the same GetCurrentLangText() mechanism so ruleset.c needs no
      separate language detection. */
   STR_LANG_CODE,

   STR_COUNT
} StringKey;

typedef struct
{
   StringKey key;
   const char *text;
} LangEntry;

/* Registers every bundled language table. Call once from EVT_INIT,
   before any drawing. */
void i18n_init(void);

/* Current-language text for a key; never NULL (compiled-in English
   fallback). A key's %d/%s placeholders must be preserved in every
   language -- the result is used as a snprintf format string. */
const char *i18n_str(StringKey key);

/* Widest font at or below `preferred` (font_large -> medium -> small)
   whose StringWidth(text) fits box_w; never steps up, falls back to
   font_small. Leaves a font active via SetFont, so re-SetFont before
   drawing if a specific color is needed. */
ifont *i18n_fit_font(const char *text, int box_w, ifont *preferred);

#endif /* I18N_H */
