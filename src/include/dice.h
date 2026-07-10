/* Grid roll for 4x4 Boggle: 16 dice, 6 faces each. The actual
   letters come from the active ruleset (see ruleset.h); this module
   only owns the shuffle-and-roll mechanics, not the letters. */

#ifndef DICE_H
#define DICE_H

#define DICE_FACES 6

/* Shuffles the 16 dice into the grid, rolls one face per die, using
   the currently active ruleset's letters (see ruleset_active()).
   Fills g.grid. */
void dice_roll_grid(void);

#endif /* DICE_H */
