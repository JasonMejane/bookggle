/* TEST-ONLY hooks, not part of the real InkView SDK. Kept separate
   from inkview.h so that file stays a faithful mirror of the real
   SDK header's shape. */

#ifndef STUB_HOOKS_H
#define STUB_HOOKS_H

/* Simulates a change of device language for tests. Affects what
   GetCurrentLangText() resolves to on subsequent calls. */
void stub_set_current_lang(const char *lang);

/* Number of times the stub's CloseApp() has been called. Lets tests
   verify a Quit tap actually reached CloseApp() without the process
   really exiting (the stub's CloseApp() is a no-op, unlike the real
   SDK's). */
int stub_close_app_call_count(void);

/* Update counters for timer redraw tests. */
void stub_reset_update_counters(void);
int stub_full_update_call_count(void);
int stub_partial_update_call_count(void);

#endif /* STUB_HOOKS_H */
