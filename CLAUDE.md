# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project

Bookggle: a native C Boggle game for PocketBook e-readers, built on the
InkView SDK (ARM ELF binary linked against `libinkview.so`). Full
gameplay/UI reference lives in `README.md` — read it before touching
`screen_*.c`, `i18n.c`, or `ruleset.c`; it documents setup-screen
toggles, pause behavior, and the language/board-size/dice-table
relationships in detail not repeated here.

## Commands

Build the real device app (requires the PocketBook SDK):

```bash
export PBSDK=/absolute/path/to/SDK_6.3.0/SDK-B288   # must be absolute, not $HOME-relative
make                # -> bookggle.app (ARM 32-bit ELF)
make clean
```

Must run from a **native terminal**, not the VS Code integrated
terminal/sandbox — the ARM cross-compiler's `cc1` needs `libmpfr.so.4`,
which the sandbox blocks. See README's "Troubleshooting the build" for
toolchain-file fixes (clang→gcc swap, `-Werror-return-type` removal)
if `$PBSDK/share/cmake/arm_conf.cmake` needs edits.

Run the logic test suite (no SDK, no device — this is what you'll use
day to day):

```bash
cd tests && make               # build + run all 8 suites
make test-flow                 # full 4x4 game scenario
make test-dice                 # dice roll per bundled ruleset
make test-integration          # game_start() <-> dice, both board sizes
make test-i18n                 # translation completeness + fit-font
make test-ruleset              # ruleset selection logic
make test-5x5                  # full 5x5 cycle + min_word_len=4 rule
make test-timer-refresh        # running tick decrements, re-arms + redraws once
make test-game-over-blink      # grid blinks to dark at game over
make clean
```

A pass prints `[OK] ...` lines then `=== ... PASSED ===`, exit 0.
A failure aborts on the first broken `assert()` (SIGABRT) — the last
`[OK]` line shows which step failed.

There is no single-test-case runner; each suite is one C file with
sequential `assert()` checks run end-to-end. To isolate a scenario,
add/extend a step in the relevant `test_*.c` (see `README.md`
"Adding tests") rather than trying to filter within a suite.

## Architecture

**Event loop**: `InkViewMain(main_handler)` in `src/main.c`;
`main_handler(int type, int par1, int par2)` dispatches `EVT_INIT`,
`EVT_SHOW`, `EVT_KEYPRESS`, `EVT_POINTERUP`, etc.

**Shared state contract**: `src/include/game_state.h` defines the
single global `GameState g` (defined once in `game_state.c`) plus
`cur_screen` and every button hitbox extern. It's included almost
everywhere — check it first when tracing how any piece of state flows
between modules. Board size is a _runtime_ field (`g.board_size`), not
a compile-time constant: arrays are sized for the largest board
(`MAX_BOARD_SIZE 5`) and only the active `board_size × board_size`
slice is used.

**Logic/drawing split**: `game_logic.c` and `timer.c` never call
`Draw*`/`Fill*` and never include a `screen_*.h` — this is what makes
`tests/` possible (real modules link into a native binary against a
no-op InkView stub). When adding gameplay logic, keep it out of
`screen_*.c`; when adding a screen, don't put rules logic in it.

**Timer adapter**: `timer.c`'s `game_timer_callback()` returns a
`TimerTickResult`, but `SetHardTimer` requires `void(*)(int)`. The
adapter `on_timer_tick()` is declared in `timer.h`, defined in
`main.c`, and is what's actually registered — it calls
`game_timer_callback()` and decides the redraw. Test files that don't
compile `main.c` provide their own minimal `on_timer_tick()`.
`SetHardTimer` is one-shot, so `game_timer_callback()` re-arms it (via
`game_timer_start()`) on every tick that keeps the game running and
stops re-arming once the game ends.

**i18n**: every UI string is looked up by `StringKey` via `i18n_str()`
(`src/i18n.c`) and resolved against the device's system language
(`GetCurrentLangText()`) — no manual locale detection. Adding a
language = one new `i18n_strings_<code>.c` table + one line in
`i18n_init()`, no `screen_*.c` changes. Fixed-width buttons use
`i18n_fit_font()` to shrink through `font_large → font_medium →
font_small` via `StringWidth()` so longer translations don't overflow.

**Ruleset system** (`GameRuleset`, `src/include/ruleset.h`): grid dice
letters, the Q/Qu digraph rule, and minimum word length are all
per-(language, board-size), not global constants. `ruleset_select()`
takes two _independent_ inputs — device language (via
`i18n_str(STR_LANG_CODE)`, piggybacking on i18n's own resolution) and
`selected_board_size` (a deliberate player choice, not device-derived)
— because a French speaker choosing 5×5 shouldn't get an English
board or vice versa. Four rulesets ship
(`ruleset_en_4x4.c`/`_en_5x5.c`/`_fr_4x4.c`/`_fr_5x5.c`); adding one
means a new `ruleset_<code>_<size>.c` + a line in `ruleset_select()` +
a reference table in `test_dice.c`. `dice.c` only owns shuffle/roll
mechanics (Fisher-Yates) — it has no letters or board-size logic of
its own, just consumes the active ruleset.

**Screens**: `SCREEN_MODE_SELECT` (setup toggles + Start), `SCREEN_GAME`
(HUD/grid/word bar), `SCREEN_MULTI_END`. `input.c` routes taps/keys to
hitboxes set by the corresponding `screen_*.c`; only the Start button
calls `game_start()` (`game_logic.c`), which is the point where
`selected_mode`/`selected_board_size`/`selected_timer_seconds` become
`g.mode`/`g.board_size`/`g.time_left` and `layout_compute_grid()`
(`ui_fonts.c`) sizes the grid for that board size.

## Testing notes

`tests/stub/inkview.h` is a minimal no-op rewrite of the real SDK
header — **never** use it for the production build; the root
`Makefile`/`CMakeLists.txt` always use the SDK's own `inkview.h`.
`tests/stub/stub_hooks.h` (not part of the real SDK) exposes
`stub_set_current_lang()`, `stub_close_app_call_count()`, and
`stub_hard_timer_call_count()` for exercising language-dependent,
`CloseApp()`-reaching, and timer-re-arming behavior from a host
process. `inkview_stub.c` also counts `FullUpdate()` /
`PartialUpdate()` calls (`stub_*_update_call_count()`), which
`test_timer_refresh.c` uses to assert a running tick redraws exactly
once and re-arms the timer (and `test_game_over_blink.c` uses to
assert the grid flashes the right number of times before settling
dark). See `README.md` for exactly what each of the suites checks and
the pattern to follow when a new (language, board-size) ruleset isn't
fully sourced yet.
