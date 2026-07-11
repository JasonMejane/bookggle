/* TEST STUB — do not use for production build.
   Minimal reimplementation of the InkView types/signatures used by
   src/, just enough to compile and run the project on a regular dev
   machine (no PocketBook SDK, no device). Real PocketBook build uses
   the SDK's own inkview.h — see Makefile / CMakeLists.txt at repo root. */

#ifndef INKVIEW_H
#define INKVIEW_H

typedef void *ifont;
typedef int (*iv_handler)(int, int, int);
typedef void (*iv_timer_handler)(int);

#define EVT_INIT 0
#define EVT_SHOW 1
#define EVT_EXIT 2
#define EVT_POINTERUP 3
#define EVT_KEYPRESS 4

#define KEY_BACK 1

#define BLACK 0
#define WHITE 1
#define LGRAY 2
#define DGRAY 3

#define ALIGN_CENTER 0
#define ALIGN_LEFT 1
#define VALIGN_MIDDLE 32

#define ICON_INFORMATION 0
#define ICON_WARNING 1

int ScreenWidth(void);
int ScreenHeight(void);
ifont *OpenFont(const char *name, int size, int bold);
void CloseFont(ifont *font);
void SetFont(ifont *font, int color);

void FillArea(int x, int y, int w, int h, int color);
void DrawRect(int x, int y, int w, int h, int color);
void DrawLine(int x1, int y1, int x2, int y2, int color);
void DrawString(int x, int y, const char *text);
void DrawTextRect(int x, int y, int w, int h, const char *text, int align);
int StringWidth(const char *text);

void FullUpdate(void);
void PartialUpdate(int x, int y, int w, int h);
void stub_reset_update_counters(void);
int stub_full_update_call_count(void);
int stub_partial_update_call_count(void);

void Message(int icon, const char *title, const char *text, int ms);
void CloseApp(void);
void InkViewMain(iv_handler handler);

int SetHardTimer(const char *name, iv_timer_handler handler, int ms);

/* Language functions. Real SDK signatures (PBSDK/FRSCSDK/SDK_481).
   GetCurrentLangText resolves against the device's active system
   language (English fallback per the SDK's own header comment);
   GetLangText is not used by src/ and is not stubbed here. */
void LoadLanguage(const char *lang);
void AddTranslation(const char *label, const char *trans);
const char *GetCurrentLangText(const char *label);

#endif /* INKVIEW_H */
