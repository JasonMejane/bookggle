/* i18n: string keys, translation lookup, button-fit font sizing.
   See docs/i18n_plan_tier1.md for the full design rationale.

   NOTE: this assumes AddTranslation(label, text) registers `text`
   under whichever language LoadLanguage() last set, and that
   GetCurrentLangText(label) resolves against the DEVICE's actual
   system language (with English fallback), based on the SDK header's
   own inline comment ("return translation for current language, do
   translate on english") and the GetLangText/GetCurrentLangText
   naming split. This has not been confirmed against running
   hardware -- verify on a real device before relying on it in
   production; i18n_str() has a defensive compiled-in English
   fallback regardless, so the app degrades safely either way. */

#ifndef I18N_H
#define I18N_H

#include "inkview.h"

typedef enum {
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

    /* Not user-facing text: each language table's own "en"/"fr" code,
       resolved via the same GetCurrentLangText() device-language
       mechanism as everything else. Lets ruleset.c pick a ruleset
       without a second, separate language-detection mechanism. */
    STR_LANG_CODE,

    STR_COUNT
} StringKey;

/* One {key, text} pair. Shared shape for every bundled language
   table (i18n_strings_en.c, i18n_strings_fr.c, ...). */
typedef struct {
    StringKey key;
    const char *text;
} LangEntry;

/* Registers every bundled language's table with the SDK. Call once
   from EVT_INIT, before any drawing. */
void i18n_init(void);

/* Current-language text for a key. Never returns NULL; falls back to
   the compiled-in English table if the SDK has no translation. Any
   %d/%s placeholder in a key's text (e.g. STR_SCORE_LABEL) must be
   preserved by every language's translation -- same count, since the
   result is used directly as a snprintf format string at the call
   site. */
const char *i18n_str(StringKey key);

/* Picks the widest-fitting font at or below `preferred` (ladder:
   font_large -> font_medium -> font_small) such that StringWidth(text)
   <= box_w. Never steps UP from `preferred`, so untranslated/short
   text keeps today's exact sizing. Falls back to font_small if
   nothing fits -- treated as a translation-length bug to fix in the
   offending language table, not something the UI should silently
   clip. Leaves a font active via SetFont as a side effect of
   measuring; caller should SetFont again before drawing if a
   specific color is needed. */
ifont *i18n_fit_font(const char *text, int box_w, ifont *preferred);

#endif /* I18N_H */
