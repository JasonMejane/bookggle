# Host tests — Bookggle PocketBook

Checks the game **logic** (rules, navigation, score, timer, dice roll,
ruleset selection, board size) on your dev machine (Linux/macOS/WSL)
— no PocketBook SDK, no device. No drawing is tested, only `src/`
behavior.

## How

`src/` only touches hardware through `inkview.h` functions
(`DrawString`, `FillArea`, `SetHardTimer`...). `tests/stub/` is a
minimal no-op rewrite of that header, so the real modules link into a
binary that runs natively here.

```
tests/
├── stub/
│   ├── inkview_stub.c     no-op bodies + font/translation stub state
│   ├── inkview.h          minimal InkView header rewrite
│   └── stub_hooks.h       test-only hooks (stub_set_current_lang, stub_close_app_call_count), not in the real SDK
├── test_dice.c            dice roll, per bundled ruleset (4 rulesets x 2 checks)
├── test_flow_5x5.c        full 5x5 cycle + min_word_len=4 rule (3 checks)
├── test_flow.c            full 4x4 game scenario (20 checks)
├── test_game_start_dice.c game_start() <-> dice integration, both board sizes (2 checks)
├── test_i18n.c            translation completeness + fit-font sizing (7 checks)
├── test_ruleset.c         ruleset selection across language x board size (7 checks)
├── Makefile
└── README.md
```

⚠️ **Never use `tests/stub/inkview.h` for the production build.** The
real PocketBook build (root `Makefile` / `CMakeLists.txt`) uses the
SDK's own `inkview.h`.

`src/timer.c` is pure logic; it registers `on_timer_tick(int)` with
InkView, whose real (drawing) body lives in `src/main.c`. Since these
test binaries don't compile `main.c` (it has its own `main()`),
`test_flow.c`, `test_game_start_dice.c`, and `test_flow_5x5.c` each
provide a minimal `on_timer_tick()` that just calls
`game_timer_callback()` — enough to link, since all three call
`game_timer_callback()` directly anyway.

The stub's `OpenFont`/`SetFont`/`StringWidth` track a real per-font
size (not a constant), so `i18n_fit_font()`'s "shrink to a smaller
font that fits" logic is meaningfully exercised in `test_i18n.c` — a
naive constant-width stub couldn't tell `font_large` from
`font_small`. It's a rough proxy (`strlen(text) * font_size / 2`),
not a real font-metrics engine.

`stub_set_current_lang()` (declared in `stub_hooks.h`, not
`inkview.h`) simulates a device language change for
`GetCurrentLangText()`, since there's no real device to change
Settings on. It's test-only and kept out of `inkview.h` so that file
stays a faithful mirror of the real SDK header's shape.
`ruleset_select(board_size)` piggybacks on the exact same language
mechanism (via `i18n_str(STR_LANG_CODE)`) for its language axis, so
`stub_set_current_lang()` drives UI-string language and (half of)
ruleset selection in tests, same as on a real device — the board-size
half of the selection is just a plain function argument, no stub
needed for that axis.

`stub_close_app_call_count()` (also in `stub_hooks.h`) counts calls to
the stub's `CloseApp()`, which is otherwise a no-op — the real SDK's
`CloseApp()` exits the app, which a host test obviously can't do to
its own process, so the count is how `test_flow.c` verifies the Quit
button's tap actually reaches `CloseApp()` rather than just checking
nothing crashed.

## Requirements

`gcc` and `make` only.

## Run

From `tests/`:

```bash
make                    # build + run all 6 suites
make test-flow          # full 4x4 game scenario only
make test-dice          # dice roll, per ruleset, only
make test-integration   # game_start() <-> dice, both board sizes, only
make test-i18n          # translation completeness + fit-font only
make test-ruleset       # ruleset selection logic only
make test-5x5           # full 5x5 cycle + min_word_len=4 rule only
make clean              # remove compiled binaries (tests/bin/)
```

A passing suite prints `[OK] ...` lines then a `=== ... PASSED (n/n) ===`
summary, exit code 0. A failure aborts on the first broken `assert()`
(`SIGABRT`); the last `[OK]` line printed shows which step it was on.

## What each file checks

**`test_flow.c`** — full 4x4 game scenario, calling real module
functions end to end: init (`i18n_init()` then
`ruleset_select(DEFAULT_BOARD_SIZE)`, mirroring `main.c`'s own
`EVT_INIT` ordering), tapping the "Solo" toggle only selects it
(`selected_mode` changes, the game does **not** start yet — the key
setup-screen behavior change from earlier versions of this screen,
where tapping Solo/Multiplayer started the game directly), tapping
"Start" actually begins the game with the current selections
(`g.mode`, `g.board_size`, `g.time_left` all checked against what was
selected), select two adjacent cells (one is Q, checks the `QU`
digraph), validate a word (score + word list), revalidate it
(duplicate rejected), tapping Pause freezes the timer (a tick while
paused leaves `time_left` unchanged and never returns a
`TIMER_TICK_ENDED_*`) and makes cell touches a no-op without touching
the underlying grid data, tapping the same button again (now showing
"Play") resumes the timer from exactly the frozen value, tapping
"Quit" reaches `CloseApp()` (counted via `stub_close_app_call_count()`,
since the stub's `CloseApp()` doesn't really exit the test process),
timer to 0:00 in solo (`TimerTickResult` + `g.game_over`), timer to
0:00 in multi (`TIMER_TICK_ENDED_MULTI` + `SCREEN_MULTI_END`, grid
must disappear), touch ignored in multi mode, "Mode" button returns to
the setup screen, Back key from the setup screen doesn't crash,
tapping "5x5" and "1:30" toggles updates
`selected_board_size`/`selected_timer_seconds`, and a new game after
those toggles actually starts at `g.board_size == 5` and
`g.time_left == TIMER_SECONDS_SHORT` (full round-trip through the real
tap handlers, not just checking each toggle's own state).

**`test_dice.c`** — checks `dice_roll_grid()` against each bundled
ruleset's own dice table, once per (language, board_size) ruleset
(selected via `stub_set_current_lang()` + `ruleset_select(board_size)`,
same as production): exact letter<->die bijection on 500 rolls per
ruleset (Kuhn's algorithm — see in-file note on why this replaced an
earlier, exponential-worst-case backtracking approach), and never
more Q or Z dice per grid than that specific ruleset's own table
actually has on 2000 rolls per ruleset. Three of the four bundled
tables have exactly one die each carrying Q and Z (English 5x5 even
puts both on the same physical die); **French 5x5 genuinely has two
Q dice** — the check is parameterized per ruleset rather than
assuming "at most one" universally, specifically to accommodate this.
Each ruleset is checked against its own independently-transcribed
reference table — verified byte-for-byte identical to
`src/ruleset_en_4x4.c` / `ruleset_en_5x5.c` / `ruleset_fr_4x4.c` /
`ruleset_fr_5x5.c`'s own tables during implementation, but kept as a
separate transcription here so the test isn't just checking a table
against itself. Sources: English 4x4
[boardgames.stackexchange.com/q/48151](https://boardgames.stackexchange.com/q/48151);
English 5x5, physically verified against an owned 1979 set,
[en.wikipedia.org/wiki/Talk:Boggle](https://en.wikipedia.org/wiki/Talk:Boggle);
French 4x4 [baggle.org/regles.php](http://baggle.org/regles.php);
French 5x5 provided directly (not independently sourced/cited like
the other three).

**`test_game_start_dice.c`** — checks `game_start()` (the real
per-game entry point called from `input.c`) actually uses
`dice_roll_grid()`, not just that `dice.c` works in isolation, at
**both** board sizes: 200 games launched at `selected_board_size=4`,
then 200 more at `=5`. This test never sets a device language, so
`game_start()`'s own `ruleset_select()` call resolves to English at
whichever board size is active (via `i18n_str`'s own fallback) — it
exercises the English 4x4/5x5 rulesets specifically, not French;
French-specific dice validity (including its 2-Q characteristic) is
`test_dice.c`'s job. Every grid is checked against the `<=1 Q` /
`<=1 Z` invariant that holds for both English rulesets, and
`g.board_size` is asserted to actually match what was requested.
`game_logic.c` calls `i18n_str()` and `ruleset_active()`, so this test
also links `i18n.c` + both string tables + `ruleset.c` + all four
ruleset tables (link-time requirement only — the test itself never
triggers a warning dialog or selects French).

**`test_i18n.c`** — checks every `StringKey` resolves to non-empty
text in both bundled languages (`en`, `fr`), that an unrecognized
device language falls back to English rather than showing a blank
string, and that `i18n_fit_font()` correctly steps down the font
ladder (`font_large` → `font_medium` → `font_small`) as the button
box gets tighter relative to the text, falling back to `font_small`
(never `NULL`) when nothing fits. (`STR_TOO_SHORT_BODY` became a
`%d`-templated string during the 5x5 work — still covered by this
test's generic "non-empty for every key" check, since a template with
its placeholder intact is still non-empty text.)

**`test_ruleset.c`** — checks `ruleset_select(board_size)` /
`ruleset_active()` selection logic specifically (as distinct from "is
this dice table internally valid", covered by `test_dice.c`), across
**both** inputs: `ruleset_active()` defaults to English 4x4 before
`ruleset_select()` is ever called (never `NULL`); all four bundled
combinations (`en`+4, `en`+5, `fr`+4, `fr`+5) each select their own
matching ruleset with the right `dice_count`/`min_word_len`; an
unrecognized device language falls back to English at both board
sizes (the fallback pattern to follow if a future language/board-size
combination isn't bundled yet — see `ruleset.h`'s doc comment); all
four bundled rulesets use the Qu digraph.

**`test_flow_5x5.c`** — full 5x5 game cycle, the scenario `test_flow.c`
doesn't cover: `game_start()` under `selected_board_size=5` actually
produces `g.board_size==5`; touching row 4/col 4 (a cell that
wouldn't exist on a 4x4 board) is reachable, proving
`cell_from_coords()`'s bounds check genuinely uses `g.board_size`
rather than a stale `4`; building a 3-letter word ("QUB") and
submitting it is **rejected** as too short, because Big Boggle's
`min_word_len=4` is actually enforced (the same 3 letters would be
accepted at 4x4 — this is the one rule that most needed a real,
not-just-unit-level test, since a bug here would silently let
3-letter words through); a 4-letter word ("QUBG") from a nearby path
is accepted, with the right score.

## Adding tests

New 4x4 scenario: add a step to `test_flow.c` (call a module function,
`assert()` the expected state). New 5x5-specific scenario: add to
`test_flow_5x5.c` instead, so 4x4 and 5x5 coverage stay in separate,
individually-runnable files. New standalone test file: add a target to
`Makefile` following `test-dice`/`test-integration` — list the `src/`
modules actually needed for the link, then a build+run rule.

New language (UI strings only): add a new `i18n_strings_<code>.c`
table in `src/`, one line in `i18n_init()` (`src/i18n.c`) to register
it, and add its language code to `test_i18n.c`'s `langs[]` array to
get completeness checking for free.

New ruleset (dice letters + rules, not just UI text): add a new
`ruleset_<code>_<size>.c` in `src/` (see `src/ruleset_en_4x4.c` for
the shape), a line in `ruleset_select()` (`src/ruleset.c`) for the
new (language, board_size) combination, add its own
independently-transcribed reference table + a loop entry in
`test_dice.c`'s `REF_TABLES[]` (with its own `max_q`/`max_z` if the
new table's Q/Z placement differs from "exactly one die each" — see
`ruleset_fr_5x5.c` for why that field exists), and cite the
letter-distribution source in a comment — see `docs/i18n_plan_tier2.md`
and `docs/board_size_5x5_plan.md` for why sourcing matters here (an
unsourced dice table is a guess, not a documented game edition) and
for the fallback pattern to follow if the new combination isn't fully
sourced yet (see `test_ruleset.c`'s unknown-device-language case for
a worked example of falling back rather than guessing).
