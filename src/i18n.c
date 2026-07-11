/* String lookup + button-fit font sizing. See i18n.h. */

#include "i18n.h"
#include "game_state.h"

extern const LangEntry I18N_EN[];
extern const int I18N_EN_COUNT;
extern const LangEntry I18N_FR[];
extern const int I18N_FR_COUNT;

/* Stable string labels passed to AddTranslation/GetCurrentLangText.
   Every StringKey must have an entry here -- tests/test_i18n.c
   checks this. */
static const char *KEY_LABELS[STR_COUNT] = {
    [STR_CHOOSE_MODE] = "choose_mode",
    [STR_BTN_SOLO] = "btn_solo",
    [STR_SOLO_HINT] = "solo_hint",
    [STR_BTN_MULTIPLAYER] = "btn_multiplayer",
    [STR_MULTI_HINT] = "multi_hint",

    [STR_HUD_TITLE_SOLO] = "hud_title_solo",
    [STR_HUD_TITLE_MULTI] = "hud_title_multi",
    [STR_SCORE_LABEL] = "score_label",
    [STR_BTN_MODE] = "btn_mode",
    [STR_BTN_SUBMIT] = "btn_submit",
    [STR_BTN_CLEAR] = "btn_clear",
    [STR_WORDS_HEADER] = "words_header",
    [STR_MULTI_INGAME_HINT] = "multi_ingame_hint",

    [STR_TIMES_UP] = "times_up",
    [STR_COMPARE_LISTS] = "compare_lists",
    [STR_CANCEL_RULE] = "cancel_rule",
    [STR_BTN_NEW_GAME] = "btn_new_game",
    [STR_BTN_CHANGE_MODE] = "btn_change_mode",

    [STR_GAME_OVER_TITLE] = "game_over_title",
    [STR_SCORE_FINAL_SINGULAR] = "score_final_singular",
    [STR_SCORE_FINAL_PLURAL] = "score_final_plural",

    [STR_TOO_SHORT_TITLE] = "too_short_title",
    [STR_TOO_SHORT_BODY] = "too_short_body",
    [STR_ALREADY_FOUND_TITLE] = "already_found_title",
    [STR_ALREADY_FOUND_BODY] = "already_found_body",
    [STR_INVALID_TITLE] = "invalid_title",
    [STR_INVALID_BODY] = "invalid_body",

    [STR_LANG_CODE] = "lang_code",
};

static void register_table(const char *lang_code, const LangEntry *table, int count)
{
    LoadLanguage(lang_code);
    for (int i = 0; i < count; i++)
        AddTranslation(KEY_LABELS[table[i].key], table[i].text);
}

void i18n_init(void)
{
    register_table("en", I18N_EN, I18N_EN_COUNT);
    register_table("fr", I18N_FR, I18N_FR_COUNT);
}

static const char *english_fallback(StringKey key)
{
    for (int i = 0; i < I18N_EN_COUNT; i++)
        if (I18N_EN[i].key == key)
            return I18N_EN[i].text;

    return "";
}

const char *i18n_str(StringKey key)
{
    if (key < 0 || key >= STR_COUNT)
        return "";

    const char *text = GetCurrentLangText(KEY_LABELS[key]);

    if (text != NULL && text[0] != '\0')
        return text;
    return english_fallback(key);
}

ifont *i18n_fit_font(const char *text, int box_w, ifont *preferred)
{
    ifont *ladder[4] = {font_dice, font_large, font_medium, font_small};
    int start = 3;

    for (int i = 0; i < 4; i++)
    {
        if (ladder[i] == preferred)
        {
            start = i;
            break;
        }
    }

    for (int i = start; i < 4; i++)
    {
        SetFont(ladder[i], BLACK);
        if (StringWidth(text) <= box_w)
            return ladder[i];
    }

    return font_small;
}
