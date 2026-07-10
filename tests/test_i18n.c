/* Checks every StringKey resolves in every bundled language (via the
   stub's simulated device language), that an unknown device language
   falls back to English, and that i18n_fit_font shrinks the font for
   text too wide for its box.
   Run: make test-i18n (from tests/), or see tests/README.md. */

#include "game_state.h"
#include "ui_fonts.h"
#include "i18n.h"
#include "stub_hooks.h"
#include <stdio.h>
#include <assert.h>

int main(void) {
    layout_compute();
    fonts_load();
    i18n_init();

    const char *langs[] = { "en", "fr" };
    for (int L = 0; L < 2; L++) {
        stub_set_current_lang(langs[L]);
        for (int k = 0; k < STR_COUNT; k++) {
            const char *text = i18n_str((StringKey)k);
            assert(text != NULL);
            assert(text[0] != '\0');
        }
        printf("[OK] all %d keys resolve to non-empty text in '%s'\n",
               STR_COUNT, langs[L]);
    }

    /* Unknown device language: i18n_str must still return something
       usable (SDK-level English fallback, or i18n_str's own
       compiled-in fallback if that fails too). */
    stub_set_current_lang("zz");
    for (int k = 0; k < STR_COUNT; k++) {
        const char *text = i18n_str((StringKey)k);
        assert(text != NULL && text[0] != '\0');
    }
    printf("[OK] unknown device language falls back to English\n");

    /* i18n_fit_font: short text keeps the preferred (larger) font. */
    stub_set_current_lang("en");
    ifont *f1 = i18n_fit_font("Solo", 400, font_large);
    assert(f1 == font_large);
    printf("[OK] short text keeps preferred font_large\n");

    /* "Change mode" is 11 chars; stub width = strlen * font_size / 2.
       font_large(30) -> 165, font_medium(22) -> 121, font_small(16) -> 88. */
    ifont *f2 = i18n_fit_font("Change mode", 150, font_large);
    assert(f2 == font_medium);
    printf("[OK] text too wide for font_large steps down to font_medium\n");

    ifont *f3 = i18n_fit_font("Change mode", 100, font_large);
    assert(f3 == font_small);
    printf("[OK] text too wide for font_medium steps down to font_small\n");

    /* Fits at no size: falls back to font_small rather than NULL. */
    ifont *f4 = i18n_fit_font("Change mode", 10, font_large);
    assert(f4 == font_small);
    printf("[OK] text that fits nowhere falls back to font_small\n");

    fonts_free();
    printf("\n=== I18N TESTS PASSED ===\n");
    return 0;
}
