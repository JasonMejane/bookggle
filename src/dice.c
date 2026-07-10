/* Shuffle-and-roll mechanics for the grid. Letters come from the
   active ruleset (see ruleset.h). */

#include "dice.h"
#include "game_state.h"
#include "ruleset.h"
#include <stdlib.h>

static void shuffle_indices(int *order, int n)
{
    for (int i = n - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        int tmp = order[i];
        order[i] = order[j];
        order[j] = tmp;
    }
}

void dice_roll_grid(void)
{
    const GameRuleset *rs = ruleset_active();
    int count = rs->dice_count;
    int board_size = rs->board_size;

    int order[MAX_DICE_COUNT];

    for (int i = 0; i < count; i++)
        order[i] = i;

    shuffle_indices(order, count);

    for (int k = 0; k < count; k++)
    {
        int r = k / board_size;
        int c = k % board_size;
        const char *die = rs->dice[order[k]];
        g.grid[r][c] = die[rand() % DICE_FACES];
    }

    g.board_size = board_size;
}
