#ifndef UI_FONTS_H
#define UI_FONTS_H

#include "inkview.h"

void fonts_load(void);
void fonts_free(void);
void layout_compute(void);
void layout_compute_grid(int board_size);

#endif // UI_FONTS_H
