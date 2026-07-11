/* Shuffle-and-roll grid mechanics only; letters come from the active
   ruleset (see ruleset.h). */

#ifndef DICE_H
#define DICE_H

#define DICE_FACES 6

/* Shuffles the active ruleset's dice into g.grid, one rolled face per
   die. */
void dice_roll_grid(void);

#endif /* DICE_H */
