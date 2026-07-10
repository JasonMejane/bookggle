/* No-op bodies for inkview.h, just enough for the link to succeed
   and the game logic to run on a regular dev machine.

   OpenFont/SetFont/StringWidth track a real per-font size so
   i18n_fit_font's "shrink to a smaller font that fits" logic is
   meaningfully exercised in host tests (a naive constant-width stub
   can't tell font_large from font_small). This is a rough proxy, not
   a real font-metrics engine -- real device widths depend on actual
   glyph metrics, not this formula. */

#include "inkview.h"
#include "stub_hooks.h"
#include <string.h>

static int g_close_app_calls = 0;
static int g_full_update_calls = 0;
static int g_partial_update_calls = 0;

int ScreenWidth(void) { return 758; }
int ScreenHeight(void) { return 1024; }

#define STUB_MAX_FONTS 16
static char g_font_storage[STUB_MAX_FONTS];
static int g_font_sizes[STUB_MAX_FONTS];
static int g_font_count = 0;
static int g_active_font_size = 20;

ifont *OpenFont(const char *name, int size, int bold)
{
    (void)name;
    (void)bold;
    if (g_font_count >= STUB_MAX_FONTS)
        g_font_count = 0;
    ifont *handle = (ifont *)&g_font_storage[g_font_count];
    g_font_sizes[g_font_count] = size;
    g_font_count++;
    return handle;
}

void CloseFont(ifont *font) { (void)font; }

void SetFont(ifont *font, int color)
{
    (void)color;
    g_active_font_size = 20;
    for (int i = 0; i < g_font_count; i++)
    {
        if ((ifont *)&g_font_storage[i] == font)
        {
            g_active_font_size = g_font_sizes[i];
            return;
        }
    }
}

void FillArea(int x, int y, int w, int h, int color)
{
    (void)x;
    (void)y;
    (void)w;
    (void)h;
    (void)color;
}

void DrawRect(int x, int y, int w, int h, int color)
{
    (void)x;
    (void)y;
    (void)w;
    (void)h;
    (void)color;
}

void DrawLine(int x1, int y1, int x2, int y2, int color)
{
    (void)x1;
    (void)y1;
    (void)x2;
    (void)y2;
    (void)color;
}

void DrawString(int x, int y, const char *text)
{
    (void)x;
    (void)y;
    (void)text;
}

void DrawTextRect(int x, int y, int w, int h, const char *text, int align)
{
    (void)x;
    (void)y;
    (void)w;
    (void)h;
    (void)text;
    (void)align;
}

int StringWidth(const char *text)
{
    if (!text)
        return 0;
    return (int)strlen(text) * g_active_font_size / 2;
}

void FullUpdate(void) { g_full_update_calls++; }
void PartialUpdate(int x, int y, int w, int h)
{
    (void)x;
    (void)y;
    (void)w;
    (void)h;
    g_partial_update_calls++;
}

void Message(int icon, const char *title, const char *text, int ms)
{
    (void)icon;
    (void)title;
    (void)text;
    (void)ms;
}

void CloseApp(void) { g_close_app_calls++; }

int stub_close_app_call_count(void) { return g_close_app_calls; }

void stub_reset_update_counters(void)
{
    g_full_update_calls = 0;
    g_partial_update_calls = 0;
}

int stub_full_update_call_count(void) { return g_full_update_calls; }
int stub_partial_update_call_count(void) { return g_partial_update_calls; }

/* No real event loop — tests call module functions directly. */
void InkViewMain(iv_handler handler) { (void)handler; }

int SetHardTimer(const char *name, iv_timer_handler handler, int ms)
{
    (void)name;
    (void)handler;
    (void)ms;
    return 1;
}

/* ── Language stub ────────────────────────────────────────────────
   Flat store of {lang, label, text}. AddTranslation tags entries
   with whatever LoadLanguage() last set. GetCurrentLangText resolves
   against g_current_lang (settable via stub_set_current_lang, the
   test-only stand-in for "the device's actual language"), falling
   back to "en" -- mirroring the real SDK's own documented English
   fallback -- then to NULL if truly nothing is registered. */

#define STUB_MAX_TRANSLATIONS 256

typedef struct
{
    char lang[8];
    char label[64];
    char text[256];
} StubTranslation;

static StubTranslation g_translations[STUB_MAX_TRANSLATIONS];
static int g_translation_count = 0;
static char g_load_lang[8] = "en";
static char g_current_lang[8] = "en";

static void stub_copy(char *dst, size_t dst_size, const char *src)
{
    strncpy(dst, src, dst_size - 1);
    dst[dst_size - 1] = '\0';
}

void LoadLanguage(const char *lang)
{
    if (!lang)
        return;
    stub_copy(g_load_lang, sizeof(g_load_lang), lang);
}

void AddTranslation(const char *label, const char *trans)
{
    if (!label || !trans)
        return;
    if (g_translation_count >= STUB_MAX_TRANSLATIONS)
        return;
    StubTranslation *t = &g_translations[g_translation_count++];
    stub_copy(t->lang, sizeof(t->lang), g_load_lang);
    stub_copy(t->label, sizeof(t->label), label);
    stub_copy(t->text, sizeof(t->text), trans);
}

static const char *find_translation(const char *lang, const char *label)
{
    for (int i = 0; i < g_translation_count; i++)
    {
        if (strcmp(g_translations[i].lang, lang) == 0 &&
            strcmp(g_translations[i].label, label) == 0)
        {
            return g_translations[i].text;
        }
    }
    return NULL;
}

const char *GetCurrentLangText(const char *label)
{
    if (!label)
        return NULL;
    const char *text = find_translation(g_current_lang, label);
    if (text != NULL)
        return text;
    return find_translation("en", label);
}

void stub_set_current_lang(const char *lang)
{
    if (!lang)
        return;
    stub_copy(g_current_lang, sizeof(g_current_lang), lang);
}
