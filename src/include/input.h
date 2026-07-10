/* Touch and key event routing for all three screens. */

#ifndef INPUT_H
#define INPUT_H

/* Routes a tap at (x, y) to the current screen, fires actions/redraws. */
void input_handle_pointerup(int x, int y);

/* KEY_BACK only: back to mode select, or close app if already there. */
void input_handle_keypress(int key);

#endif /* INPUT_H */
