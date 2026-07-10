/* Checks dice_roll_grid() against each bundled ruleset's own dice
   table, once per (language, board_size) ruleset:
   1. exact letter<->die bijection (each die used once)
   2. never more than that ruleset's own expected number of Q or Z
      dice per grid. Three of the four bundled rulesets have exactly
      one die each carrying Q and Z; French 5x5 genuinely has TWO
      dice carrying Q (see ruleset_fr_5x5.c) -- this is a real
      characteristic of that table, not a bug, so the expected max is
      a per-ruleset value rather than a universal "at most one".

   Bijection is checked via Kuhn's algorithm (augmenting-path bipartite
   matching between letter positions and dice), not naive backtracking.
   An earlier version of this test used plain DFS-with-backtracking,
   which is correct but has no bound on worst-case time -- on the
   25-dice English 5x5 table (heavy letter repetition, including a
   literal duplicate die), some random rolls took over two minutes to
   resolve, hanging the test suite. Kuhn's algorithm is O(V*E) always
   (here: 25 * 150 at most), so this can never happen regardless of
   how adversarial a given roll's letter arrangement is.

   Run: make test-dice (from tests/), or see tests/README.md. */

#include "game_state.h"
#include "dice.h"
#include "ruleset.h"
#include "i18n.h"
#include "stub_hooks.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct
{
    const char *lang_code;
    int board_size;
    int dice_count;
    int max_q; /* how many dice carry Q in this table */
    int max_z; /* how many dice carry Z in this table */
    const char dice[MAX_DICE_COUNT][6];
} RefTable;

/* Independent copies of each ruleset's dice table (must match
   src/ruleset_en_4x4.c / src/ruleset_en_5x5.c / src/ruleset_fr_4x4.c /
   src/ruleset_fr_5x5.c), kept separate so this test checks
   dice_roll_grid() against a reference written elsewhere, not
   against its own internal table. */
static const RefTable REF_TABLES[] = {
    {"en", 4, 16, 1, 1, {
                            {'A', 'A', 'E', 'E', 'G', 'N'},
                            {'A', 'B', 'B', 'J', 'O', 'O'},
                            {'A', 'C', 'H', 'O', 'P', 'S'},
                            {'A', 'F', 'F', 'K', 'P', 'S'},
                            {'A', 'O', 'O', 'T', 'T', 'W'},
                            {'C', 'I', 'M', 'O', 'T', 'U'},
                            {'D', 'E', 'I', 'L', 'R', 'X'},
                            {'D', 'E', 'L', 'R', 'V', 'Y'},
                            {'D', 'I', 'S', 'T', 'T', 'Y'},
                            {'E', 'E', 'G', 'H', 'N', 'W'},
                            {'E', 'E', 'I', 'N', 'S', 'U'},
                            {'E', 'H', 'R', 'T', 'V', 'W'},
                            {'E', 'I', 'O', 'S', 'S', 'T'},
                            {'E', 'L', 'R', 'T', 'T', 'Y'},
                            {'H', 'I', 'M', 'N', 'Q', 'U'},
                            {'H', 'L', 'N', 'N', 'R', 'Z'},
                        }},
    {"en", 5, 25, 1, 1, {
                            {'A', 'A', 'A', 'F', 'R', 'S'},
                            {'A', 'A', 'E', 'E', 'E', 'E'},
                            {'A', 'A', 'F', 'I', 'R', 'S'},
                            {'A', 'D', 'E', 'N', 'N', 'N'},
                            {'A', 'E', 'E', 'E', 'E', 'M'},
                            {'A', 'E', 'E', 'G', 'M', 'U'},
                            {'A', 'E', 'G', 'M', 'N', 'N'},
                            {'A', 'F', 'I', 'R', 'S', 'Y'},
                            {'B', 'J', 'K', 'Q', 'X', 'Z'},
                            {'C', 'C', 'E', 'N', 'S', 'T'},
                            {'C', 'E', 'I', 'I', 'L', 'T'},
                            {'C', 'E', 'I', 'L', 'P', 'T'},
                            {'C', 'E', 'I', 'P', 'S', 'T'},
                            {'D', 'D', 'H', 'N', 'O', 'T'},
                            {'D', 'H', 'H', 'L', 'O', 'R'},
                            {'D', 'H', 'L', 'N', 'O', 'R'},
                            {'D', 'H', 'L', 'N', 'O', 'R'},
                            {'E', 'I', 'I', 'I', 'T', 'T'},
                            {'E', 'M', 'O', 'T', 'T', 'T'},
                            {'E', 'N', 'S', 'S', 'S', 'U'},
                            {'F', 'I', 'P', 'R', 'S', 'Y'},
                            {'G', 'O', 'R', 'R', 'V', 'W'},
                            {'I', 'P', 'R', 'R', 'R', 'Y'},
                            {'N', 'O', 'O', 'T', 'U', 'W'},
                            {'O', 'O', 'O', 'T', 'T', 'U'},
                        }},
    {"fr", 4, 16, 1, 1, {
                            {'E', 'T', 'U', 'K', 'N', 'O'},
                            {'E', 'V', 'G', 'T', 'I', 'N'},
                            {'D', 'E', 'C', 'A', 'M', 'P'},
                            {'I', 'E', 'L', 'R', 'U', 'W'},
                            {'E', 'H', 'I', 'F', 'S', 'E'},
                            {'R', 'E', 'C', 'A', 'L', 'S'},
                            {'E', 'N', 'T', 'D', 'O', 'S'},
                            {'O', 'F', 'X', 'R', 'I', 'A'},
                            {'N', 'A', 'V', 'E', 'D', 'Z'},
                            {'E', 'I', 'O', 'A', 'T', 'A'},
                            {'G', 'L', 'E', 'N', 'Y', 'U'},
                            {'B', 'M', 'A', 'Q', 'J', 'O'},
                            {'T', 'L', 'I', 'B', 'R', 'A'},
                            {'S', 'P', 'U', 'L', 'T', 'E'},
                            {'A', 'I', 'M', 'S', 'O', 'R'},
                            {'E', 'N', 'H', 'R', 'I', 'S'},
                        }},
    {"fr", 5, 25, 2, 1, {
                            {'M', 'D', 'N', 'S', 'N', 'H'},
                            {'G', 'F', 'S', 'T', 'E', 'Y'},
                            {'L', 'M', 'T', 'R', 'X', 'S'},
                            {'T', 'T', 'R', 'S', 'C', 'H'},
                            {'B', 'M', 'L', 'N', 'D', 'L'},
                            {'T', 'M', 'R', 'D', 'B', 'T'},
                            {'E', 'I', 'U', 'E', 'A', 'O'},
                            {'R', 'L', 'X', 'S', 'S', 'B'},
                            {'N', 'A', 'A', 'T', 'E', 'Q'},
                            {'T', 'C', 'J', 'F', 'S', 'H'},
                            {'I', 'E', 'E', 'A', 'O', 'A'},
                            {'N', 'D', 'H', 'S', 'N', 'M'},
                            {'I', 'A', 'A', 'I', 'E', 'O'},
                            {'O', 'E', 'U', 'E', 'I', 'A'},
                            {'L', 'C', 'P', 'R', 'J', 'S'},
                            {'D', 'S', 'T', 'L', 'S', 'M'},
                            {'N', 'K', 'L', 'P', 'F', 'N'},
                            {'D', 'W', 'R', 'N', 'L', 'P'},
                            {'R', 'Z', 'N', 'N', 'T', 'Q'},
                            {'R', 'G', 'L', 'R', 'V', 'F'},
                            {'R', 'V', 'C', 'G', 'R', 'T'},
                            {'I', 'I', 'O', 'E', 'A', 'E'},
                            {'E', 'U', 'I', 'A', 'E', 'O'},
                            {'U', 'I', 'A', 'E', 'O', 'A'},
                            {'N', 'S', 'E', 'V', 'A', 'E'},
                        }},
};
#define REF_TABLE_COUNT (int)(sizeof(REF_TABLES) / sizeof(REF_TABLES[0]))

static int letter_on_die(const RefTable *ref, int die_idx, char letter)
{
    for (int f = 0; f < 6; f++)
        if (ref->dice[die_idx][f] == letter)
            return 1;
    return 0;
}

/* Kuhn's algorithm: match_die[d] holds which letter position die d
   is currently assigned to (-1 = unassigned). try_kuhn attempts to
   find an augmenting path so letter_idx gets a die, displacing a
   previous assignment (and recursively re-homing it) if needed. */
static int match_die[MAX_DICE_COUNT];

static int try_kuhn(const RefTable *ref, const char *letters,
                    int letter_idx, int *visited)
{
    for (int d = 0; d < ref->dice_count; d++)
    {
        if (visited[d] || !letter_on_die(ref, d, letters[letter_idx]))
            continue;
        visited[d] = 1;
        if (match_die[d] == -1 || try_kuhn(ref, letters, match_die[d], visited))
        {
            match_die[d] = letter_idx;
            return 1;
        }
    }
    return 0;
}

static int has_bijection(const RefTable *ref, const char *letters, int n)
{
    for (int d = 0; d < ref->dice_count; d++)
        match_die[d] = -1;
    for (int i = 0; i < n; i++)
    {
        int visited[MAX_DICE_COUNT] = {0};
        if (!try_kuhn(ref, letters, i, visited))
            return 0;
    }
    return 1;
}

static void check_bijection(const RefTable *ref, int total_runs)
{
    for (int run = 0; run < total_runs; run++)
    {
        dice_roll_grid();

        char letters[MAX_DICE_COUNT];
        int k = 0;
        for (int r = 0; r < ref->board_size; r++)
            for (int c = 0; c < ref->board_size; c++)
                letters[k++] = g.grid[r][c];

        if (!has_bijection(ref, letters, ref->dice_count))
        {
            fprintf(stderr,
                    "[FAIL '%s' %dx%d run %d] no valid letter<->die bijection found!\n",
                    ref->lang_code, ref->board_size, ref->board_size, run);
            exit(1);
        }
    }
    printf("[OK] '%s' %dx%d: %d rolls checked, each uses exactly its %d "
           "official dice, one each, with a valid face.\n",
           ref->lang_code, ref->board_size, ref->board_size, total_runs,
           ref->dice_count);
}

static void check_qz_uniqueness(const RefTable *ref, int total_runs)
{
    int max_q = 0, max_z = 0;
    for (int run = 0; run < total_runs; run++)
    {
        dice_roll_grid();
        int q = 0, z = 0;
        for (int r = 0; r < ref->board_size; r++)
            for (int c = 0; c < ref->board_size; c++)
            {
                if (g.grid[r][c] == 'Q')
                    q++;
                if (g.grid[r][c] == 'Z')
                    z++;
            }
        if (q > max_q)
            max_q = q;
        if (z > max_z)
            max_z = z;
        if (q > ref->max_q)
        {
            fprintf(stderr,
                    "[FAIL '%s' %dx%d run %d] Q count %d exceeds expected max %d!\n",
                    ref->lang_code, ref->board_size, ref->board_size, run, q, ref->max_q);
            exit(1);
        }
        if (z > ref->max_z)
        {
            fprintf(stderr,
                    "[FAIL '%s' %dx%d run %d] Z count %d exceeds expected max %d!\n",
                    ref->lang_code, ref->board_size, ref->board_size, run, z, ref->max_z);
            exit(1);
        }
    }
    printf("[OK] '%s' %dx%d: %d rolls, never more than %d Q (observed max=%d) "
           "or %d Z (observed max=%d) per grid.\n",
           ref->lang_code, ref->board_size, ref->board_size, total_runs,
           ref->max_q, max_q, ref->max_z, max_z);
}

int main(void)
{
    srand((unsigned)time(NULL));
    i18n_init();

    for (int i = 0; i < REF_TABLE_COUNT; i++)
    {
        const RefTable *ref = &REF_TABLES[i];
        stub_set_current_lang(ref->lang_code);
        ruleset_select(ref->board_size);

        const GameRuleset *active = ruleset_active();
        assert(strcmp(active->lang_code, ref->lang_code) == 0);
        assert(active->board_size == ref->board_size);
        assert(active->dice_count == ref->dice_count);

        check_bijection(ref, 500);
        check_qz_uniqueness(ref, 2000);
    }

    printf("=== DICE TESTS PASSED ===\n\n");
    return 0;
}
