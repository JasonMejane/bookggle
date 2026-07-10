/* Setup screen: startup, and return target for "Mode"/Back. Shows
   Mode/Board-size/Timer toggles plus a Start button -- toggles only
   change a selection, Start is the only control that begins a game. */

#ifndef SCREEN_MODE_SELECT_H
#define SCREEN_MODE_SELECT_H

/* Draws title + Mode/Board-size/Timer toggles + Start button,
   triggers FullUpdate. All toggle and Start hitboxes are written to
   their *_btn_x/y/w/h globals here and read directly from there in
   input.c, same pattern as the HUD's mode_btn_... fields. */
void draw_mode_select(void);

#endif /* SCREEN_MODE_SELECT_H */
