/* Setup screen: startup, and return target for "Mode"/Back. */

#ifndef SCREEN_MODE_SELECT_H
#define SCREEN_MODE_SELECT_H

/* Draws the title, the three toggle rows, and Start; writes all
   hitboxes to their *_btn_ globals (read by input.c). FullUpdate. */
void draw_mode_select(void);

#endif /* SCREEN_MODE_SELECT_H */
