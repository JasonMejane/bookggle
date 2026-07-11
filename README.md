# Bookggle - Boggle for PocketBook — Developer Guide

Bookggle is a native C Boggle game for PocketBook e-readers, built on the InkView SDK.

## Quick start

```bash
# Build the real PocketBook app (needs the SDK, see "PocketBook SDK" below)
export PBSDK=$HOME/SDK_6.3.0
make

# Run the full logic test suite on your dev machine (no SDK, no device)
cd tests && make
```

See **"Build"** for CMake/Docker alternatives and how to deploy to a
device, and **"Run logic without a device"** / `tests/README.md` for
what each test suite checks.

## Setup screen

Before a game starts, a setup screen shows three independent toggles
— **Mode** (Solo / Multiplayer), **Grid size** (4×4 / 5×5), **Timer**
(1:30 / 3:00) — plus a **Start** button. Toggles only change a
selection (`selected_mode`, `selected_board_size`,
`selected_timer_seconds`, all in `src/include/game_state.h`); nothing
begins until Start is tapped. This is deliberate: tapping "Solo" no
longer jumps straight into a game the way earlier versions of this
screen did, so a mis-tap while adjusting one setting can't
accidentally start a mismatched game with the others still at their
old values.

- **Solo** — tap cells to spell words, live score, word list.
- **Multiplayer** — only the grid and timer are shown. Each player
  notes words on paper. At 0:00 the **grid disappears** and an end
  screen tells players to compare lists (words found by more than one
  player cancel out — standard Boggle rule).
- **Grid size** — 4×4 (standard) or 5×5 ("Big Boggle"), independent of
  Solo/Multiplayer.
- **Timer** — 1:30 or 3:00, independent of the other two. Read by
  `game_start()` (`src/game_logic.c`) via `selected_timer_seconds`
  instead of the old hardcoded 3-minute constant.

The setup screen is shown at startup, or anytime via the **"Mode"**
button (top-right of the in-game HUD), which returns to it without
losing the current selections. The physical **Back** key does the
same (or closes the app if already on that screen).

## Game screen

Top to bottom: HUD (title, timer, score in Solo), the grid, then a
**Pause/Play** button, then the word bar (Solo) or a hint (Multiplayer).

- **Pause** freezes the timer and hides every cell's letter (the grid
  outline stays visible, just blank) without losing any progress.
  **Play** resumes the timer from exactly where it was and shows the
  letters again. The button's own label flips between the two —
  "Pause" while running, "Play" while paused — so it always describes
  what tapping it will do next, not the current state.
- While paused, touching a cell does nothing (same guard as
  Multiplayer mode or after time runs out) — the underlying grid data
  is never touched by pausing, only what's drawn and what input does
  with it.
- Submit/Clear stay active while paused, since finishing a word you
  already built before pausing doesn't reveal anything new.

Two buttons sit in the HUD's top-right corner: **"Mode"** (returns to
the setup screen; tapping "Start" again from there begins a brand new
game, not a resume — the in-progress game's state isn't preserved
once you leave this screen) and **"Quit"** (closes the app
immediately, no confirmation prompt, same directness as every other
button in this app).

## Language follows the device; board size is a separate player choice

Every menu, button, and dialog is looked up by key (`src/i18n.c` /
`src/include/i18n.h`) and resolved against the device's current
system language automatically via the InkView SDK's
`GetCurrentLangText()` — no manual locale detection in the app. Two
languages ship today: English (`src/i18n_strings_en.c`) and French
(`src/i18n_strings_fr.c`). Adding another is one new table file plus
one line in `i18n_init()` — no changes to any `screen_*.c` file.

Buttons with fixed pixel widths (Solo/Multiplayer/Submit/Clear/Mode/
Quit/Pause/Play/Start/New game/Change mode) use `i18n_fit_font()` to
shrink through the font ladder (`font_large` → `font_medium` →
`font_small`) via `StringWidth()` measurement, so a longer translation
doesn't overflow its box. Grid cell letters use the same measurement,
since a 5×5 board's smaller cells can make even a single letter (or
the "Qu" digraph) tight.

The **grid roll and word rules also follow a ruleset** (`GameRuleset`,
`src/include/ruleset.h`) — not just the menu text. But unlike the UI
language, **board size is a deliberate player choice, not something
that follows the device automatically** — a French-speaking player
picking the bigger grid still expects a French UI, and a device being
in French doesn't imply a player wants (or doesn't want) 5×5. So
`ruleset_select(board_size)` takes two independent inputs: the device
language (same signal as the UI strings, via `i18n_str(STR_LANG_CODE)`)
and the board size the player picked. `ruleset_active()` returns the
result everywhere the game needs dice letters, the Q/Qu digraph rule,
or the minimum word length.

Four rulesets ship today:

```
English 4x4 ("New Boggle" 16-dice set):
AAEEGN ABBJOO ACHOPS AFFKPS AOOTTW CIMOTU DEILRX DELRVY
DISTTY EEGHNW EEINSU EHRTVW EIOSST ELRTTY HIMNQU HLNNRZ

English 5x5 ("Big Boggle" 25-dice set):
AAAFRS AAEEEE AAFIRS ADENNN AEEEEM AEEGMU AEGMNN AFIRSY
BJKQXZ CCENST CEIILT CEILPT CEIPST DDHNOT DHHLOR DHLNOR
DHLNOR EIIITT EMOTTT ENSSSU FIPRSY GORRVW IPRRRY NOOTUW
OOOTTU

French 4x4 (B@ggle, baggle.org/regles.php):
ETUKNO EVGTIN DECAMP IELRUW EHIFSE RECALS ENTDOS OFXRIA
NAVEDZ EIOATA GLENYU BMAQJO TLIBRA SPULTE AIMSOR ENHRIS

French 5x5 (Big B@ggle, 25-dice set):
MDNSNH GFSTEY LMTRXS TTRSCH BMLNDL TMRDBT EIUEAO RLXSSB
NAATEQ TCJFSH IEEAOA NDHSNM IAAIEO OEUEIA LCPRJS DSTLSM
NKLPFN DWRNLP RZNNTQ RGLRVF RVCGRT IIOEAE EUIAEO UIAEOA
NSEVAE
```

Each line above is a 6-face die. `dice_roll_grid()` in **`src/dice.c`**
shuffles the active ruleset's dice (Fisher-Yates) into the grid, then
rolls one face per die — unlike picking from a flat letter-frequency
bag, this guarantees a bounded, ruleset-specific number of Q and Z
dice per grid rather than an unbounded one. Three of the four bundled
tables have exactly one die each carrying Q and one carrying Z (the
English 5×5 set even puts both on the same physical die, confirmed by
its sourcing trail, not a transcription bug); the **French 5×5 table
genuinely has two dice carrying Q** (`tests/test_dice.c` checks each
ruleset against its own expected count rather than assuming "at most
one" universally). All four rulesets merge Q as **"Qu"**: the cell
displays `Qu`, selecting it inserts the `QU` digraph into the word
being built, and scoring counts **cells** walked (not characters).
This is a per-ruleset flag (`has_qu_digraph`), not hardcoded.

The 4×4 and 5×5 rulesets deliberately differ in minimum word length
— **3 letters at 4×4, 4 letters at 5×5** — confirmed by multiple
official Big Boggle rule sheets and by baggle.org's own Big B@ggle
rules text for French. This is a per-ruleset field (`min_word_len`),
read at validation time (`ruleset_active()->min_word_len` in
`game_logic.c`), so the "too short" dialog is templated with `%d`
rather than hardcoding either number.

Board size itself is **not** a compile-time constant — `GameState`'s
grid/selection arrays are sized for the larger board (5×5) and a
runtime `g.board_size` field tracks how much of them is in play (see
`MAX_BOARD_SIZE`/`MAX_DICE_COUNT` in `src/include/game_state.h`).
Screen layout (`CELL_SIZE`/`GRID_X`/`GRID_Y`) is computed per game,
after the board size is known (`layout_compute_grid()`, called from
`game_start()`), with a board-size-aware width budget (60% of screen
width at 4×4, 75% at 5×5) so touch targets stay a comparable physical
size on both boards rather than shrinking with the extra column.

Adding a language's _rules_ (not just its menu text) means a new
`ruleset_<code>_<size>.c` data file plus a line in `ruleset_select()`
— see `docs/i18n_plan_tier2.md` for the original 4×4-only design
rationale and `docs/board_size_5x5_plan.md` for the board-size
extension, including the full citation trail for both English dice
tables and the Big Boggle rule confirmations.

---

## InkView SDK essentials

PocketBook apps are ARM ELF binaries linked against `libinkview.so`.
The SDK provides the cross-compiler and headers.

```c
int main(int argc, char **argv) {
    InkViewMain(main_handler);
    return 0;
}
```

`InkViewMain` runs the event loop; your code lives in
`main_handler(int type, int par1, int par2)`.

| Event             | Trigger               | par1 / par2        |
| ----------------- | --------------------- | ------------------ |
| `EVT_INIT`        | App startup           | —                  |
| `EVT_SHOW`        | Screen needs redraw   | —                  |
| `EVT_EXIT`        | App closing           | —                  |
| `EVT_KEYPRESS`    | Physical key pressed  | par1 = key code    |
| `EVT_POINTERUP`   | Finger lifted (touch) | par1 = X, par2 = Y |
| `EVT_POINTERDOWN` | Finger pressed        | par1 = X, par2 = Y |

```c
FillArea(x, y, w, h, WHITE);

ifont *f = OpenFont("LiberationSans", 24, 1);
SetFont(f, BLACK);
DrawString(x, y, "Hello!");
DrawTextRect(x, y, w, h, "centered text", ALIGN_CENTER);

DrawRect(x, y, w, h, BLACK);

FullUpdate();           // full refresh, slow, clean
PartialUpdate(x,y,w,h);  // fast, may ghost
```

> On e-ink, whatever is drawn stays until you cover it. Always
> `FillArea(..., WHITE)` before redrawing.

```c
SetHardTimer("MyTimer", my_callback, 1000);

static void my_callback(int id) {
    // ... update + PartialUpdate(...)
}
```

> Here, the countdown lives in **`src/timer.c`** and draws nothing —
> it returns a `TimerTickResult`. The adapter `on_timer_tick()` in
> **`src/main.c`** is the one actually registered with `SetHardTimer`
> (which requires `void(*)(int)`), and it picks the redraw.

---

## Project layout

One `.c` / `.h` pair per concern, headers in `src/include/`.

```
bookggle/
├── src/
│   ├── include/
│   │   ├── dice.h                shuffle-and-roll mechanics only
│   │   ├── game_logic.h          rules: grid, selection, score, Q/Qu
│   │   ├── game_state.h          shared types, constants, extern state
│   │   ├── i18n.h                string keys, translation lookup, fit-font
│   │   ├── input.h               touch/key routing
│   │   ├── ruleset.h             per-(language, board size) dice/digraph/word-length rules
│   │   ├── screen_game.h
│   │   ├── screen_mode_select.h
│   │   ├── screen_multi_end.h
│   │   ├── timer.h               game timer
│   │   └── ui_fonts.h            font loading + screen layout
│   ├── dice.c              shuffle-and-roll mechanics only, no letters
│   ├── game_logic.c       pure logic, no drawing
│   ├── game_state.c       single definition of global state `g`
│   ├── i18n_strings_en.c  English string table
│   ├── i18n_strings_fr.c  French string table
│   ├── i18n.c             registration + lookup + fit-font
│   ├── input.c
│   ├── main.c              main_handler + main()
│   ├── ruleset_en_4x4.c   English 4x4 dice table + rules
│   ├── ruleset_en_5x5.c   English 5x5 ("Big Boggle") dice table + rules
│   ├── ruleset_fr_4x4.c   French 4x4 dice table + rules
│   ├── ruleset_fr_5x5.c   French 5x5 ("Big B@ggle") dice table + rules
│   ├── ruleset.c          selects active ruleset (language x board size)
│   ├── screen_game.c
│   ├── screen_mode_select.c
│   ├── screen_multi_end.c
│   ├── timer.c            pure logic, no drawing
│   └── ui_fonts.c
├── tests/             host tests, no SDK/device — see tests/README.md
├── CMakeLists.txt
├── Makefile
└── README.md
```

`game_state.h` is the shared-data contract, included almost
everywhere. `game_logic.c` and `timer.c` never call `Draw*`/`Fill*`
and never include a `screen_*.h` — pure logic, testable without
drawing (see `tests/`). `game_logic.c` calls `i18n_str()` (for its
warning dialogs), `ruleset_active()` (for the Q/Qu digraph and
minimum word length), and `layout_compute_grid()` (from `game_start()`,
once the board size is known), so it depends on `i18n.h`, `ruleset.h`,
and `ui_fonts.h`, but those are lookups and layout math, not drawing
calls. `dice.c` depends on `ruleset.h` too — it owns the shuffle/roll
mechanics only, not any specific set of letters or a specific board
size.

`timer.c` can't register directly with `SetHardTimer` (its
`game_timer_callback` returns `TimerTickResult`, not `void`), so it
registers `on_timer_tick(int)` instead — declared in `timer.h`,
defined in `main.c`, which calls `game_timer_callback()` and chooses
the redraw. This keeps timer rules and screen rendering independently
changeable.

`ruleset.c` picks the active ruleset from **two** independent inputs:
device language (reusing `i18n`'s own resolution — each language
table carries one extra, non-user-facing entry, `STR_LANG_CODE`, whose
translated value is simply its own language code, so `ruleset_select()`
reads it through `i18n_str()` rather than a second detection
mechanism) and the board size the player picked
(`selected_board_size`, set by the mode-select screen's toggle). It's
deliberately not a single combined lookup — see the README's
"Language follows the device" section above for why board size and
language are kept independent.

---

## PocketBook SDK

Get the SDK: https://github.com/pocketbook/SDK_6.3.0

Or use the Docker image:

```bash
docker pull 5keeve/pocketbook-sdk:6.3.0-b288-v1
```

## Build

### Build from a native terminal (recommended)

The project requires the PocketBook SDK and a cross-compiler for ARM. Building must happen **outside the VS Code sandbox** to access system libraries (libmpfr.so.4) that the cross-compiler internally depends on.

**Steps:**

1. **Install the SDK** from https://github.com/pocketbook/SDK_6.3.0 or download the pre-built tarball.

2. **Open a native terminal** (not VS Code's integrated terminal) and set the SDK path:

```bash
export PBSDK=/path/to/SDK_6.3.0/SDK-B288
```

Use the **absolute path** to your SDK installation. Relative paths (like `$HOME/SDK_6.3.0`) may not resolve correctly when invoked from CMake.

3. **Build:**

```bash
cd /path/to/bookggle
rm -rf build
make
```

This runs CMake with the toolchain file (`$PBSDK/share/cmake/arm_conf.cmake`), which configures the ARM cross-compiler (gcc 6.3.0) and generates the ARM 32-bit ELF binary `bookggle.app`.

**Verify the build:**

```bash
file bookggle.app
# -> ELF 32-bit LSB executable, ARM, EABI5 version 1 (SYSV), dynamically linked
```

**Why VS Code's terminal doesn't work:**

VS Code runs in a sandbox that blocks access to system libraries. The ARM cross-compiler requires `libmpfr.so.4` (a GNU multi-precision floating-point library used internally by gcc's cc1 backend), which the sandbox cannot access. Building in a native terminal grants full filesystem and library access, allowing the cross-compiler to run successfully.

### CMake (manual)

If you prefer to invoke CMake directly:

```bash
export PBSDK=/path/to/SDK_6.3.0/SDK-B288
rm -rf build
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=$PBSDK/share/cmake/arm_conf.cmake
make
```

### Docker (no local SDK)

If you don't have the SDK installed locally:

```bash
docker run --rm -v $(pwd):/project -w /project \
  5keeve/pocketbook-sdk:6.3.0-b288-v1 \
  sh -c 'arm-obreey-linux-gnueabi-gcc -I src/include src/*.c \
         -o bookggle.app -linkview -lm'
```

**Note:** This Docker image contains ARM binaries, so it must run on an ARM host or a system with cross-architecture emulation (e.g., QEMU/binfmt). Without it, you may see "cannot execute binary file" errors.

## Run logic without a device

`tests/` provides a no-op InkView stub so the game logic builds and
runs on a regular dev machine:

```bash
cd tests
make
```

Six suites: `test_flow.c` (full 4x4 game scenario, 20 checks),
`test_dice.c` (dice roll, bijection + Q/Z uniqueness, once per bundled
ruleset — en-4x4/en-5x5/fr-4x4/fr-5x5), `test_game_start_dice.c` (game_start
<-> dice integration, both board sizes), `test_i18n.c` (translation
completeness across languages + fit-font sizing), `test_ruleset.c`
(ruleset selection across all 4 bundled language x board-size pairs,
plus the unknown-language fallback), `test_flow_5x5.c` (full 5x5 cycle
and the min_word_len=4 rule). See `tests/README.md` for details.

### Troubleshooting the build

**Issue: CMake can't find the toolchain file**

The toolchain path must be an **absolute path**. If you set:

```bash
export PBSDK=$HOME/SDK_6.3.0/SDK-B288
cmake .. -DCMAKE_TOOLCHAIN_FILE=$PBSDK/share/cmake/arm_conf.cmake
```

The `$HOME` variable is expanded at the time CMake runs, not when you export it. If CMake is invoked from a different context (e.g., via `make` from another directory), it may not resolve correctly. Use an explicit absolute path:

```bash
export PBSDK=/home/incien/SDK_6.3.0/SDK-B288
```

**Issue: "cc1: error while loading shared libraries: libmpfr.so.4"**

This means the ARM cross-compiler cannot find runtime libraries it depends on. This happens **only in the VS Code sandbox**. Solution: build in a native terminal (not VS Code's integrated terminal).

If you must build in VS Code, try setting `LD_LIBRARY_PATH` before building:

```bash
export LD_LIBRARY_PATH=$PBSDK/usr/lib:$PBSDK/usr/lib/arm-obreey-linux-gnueabi:$LD_LIBRARY_PATH
make
```

If this still fails in VS Code, the sandbox is blocking access. Open a native terminal instead.

**Issue: "arm-obreey-linux-gnueabi-clang: command not found" or clang-related errors**

The SDK's default toolchain file may specify clang instead of gcc. Edit `$PBSDK/share/cmake/arm_conf.cmake` and change:

```cmake
set(CMAKE_C_COMPILER arm-obreey-linux-gnueabi-clang)
set(CMAKE_CXX_COMPILER arm-obreey-linux-gnueabi-clang++)
```

to:

```cmake
set(CMAKE_C_COMPILER arm-obreey-linux-gnueabi-gcc)
set(CMAKE_CXX_COMPILER arm-obreey-linux-gnueabi-g++)
```

**Issue: "unrecognized command line option '-Werror-return-type'"**

Some older SDK toolchain files contain compiler flags that gcc 6.3.0 doesn't support. Edit `$PBSDK/share/cmake/arm_conf.cmake` and remove `-Werror-return-type` from `CMAKE_C_FLAGS`.

## Deploy to device

### Basic deployment

1. Connect over USB (mass storage mode)
2. Copy `bookggle.app` to `/mnt/ext1/applications/bookggle.app`
3. Disconnect, launch from the Applications menu

Or via SSH (dropbear):

```bash
scp bookggle.app root@192.168.1.X:/mnt/ext1/applications/bookggle.app
```

### Custom launcher name and icons

To give your compiled app (`bookggle.app`) a clean display name and custom icons on a PocketBook e-reader, you must register it in the device's desktop configuration. PocketBook's launcher does not read an internal app manifest from the binary; instead, it uses the hidden desktop config file `view.json` to map a user app entry to a title and icon files.

#### Step 1: Create your icons

PocketBook expects bitmap icons in the launcher configuration:

- **Format:** Windows BMP (`.bmp`)
- **Resolution:** up to `128 x 128` pixels (grayscale or monochrome is safest for e-ink)
- **Files required:** two images per app
  1. `bookggle.bmp` — the normal icon
  2. `bookggle_f.bmp` — the focused/tapped icon

Place these two files in the device's applications icons area, for example:

`/mnt/ext1/applications/icons/`

#### Step 2: Edit `view.json`

1. Connect the PocketBook by USB.
2. Enable hidden files in your file manager and open `/system/config/desktop/view.json`.
3. Add a user-app entry using the `U_` prefix, where the suffix must match the app filename without the `.app` extension:

```json
"U_Bookggle": {
    "path": "/mnt/ext1/applications/bookggle.app",
    "title": "Bookggle",
    "icon": "/mnt/ext1/applications/icons/bookggle.bmp",
    "focused_icon": "/mnt/ext1/applications/icons/bookggle_f.bmp"
}
```

#### Step 3: Add it to the apps list

In the same `view.json` file, add your entry to the visible apps array, for example:

```json
"apps": [ "PB_Dictionary", "PB_Library", "PB_Settings", "U_Bookggle" ]
```

#### Step 4: Reboot

Save the file carefully, eject the device safely, and reboot the PocketBook. The app should then appear with the intended launcher title and custom icon.

---

## Extending

**Dictionary** — `word_is_valid()` in `src/game_logic.c` only checks
length right now (against `ruleset_active()->min_word_len`). A real
dictionary needs one word list per language, not one global list —
natural fit alongside the existing per-language split: load
`dict_en.txt`/`dict_fr.txt` based on `ruleset_active()->lang_code`,
or add a `dict_path` field to `GameRuleset` itself. Sorted word list +
binary search:

```c
static char dict[50000][16];
static int  dict_size = 0;

void load_dict(void) {
    FILE *f = fopen("/mnt/ext1/applications/bookggle.dict", "r");
    if (!f) return;
    while (fgets(dict[dict_size], 16, f) && dict_size < 50000) {
        dict[dict_size][strcspn(dict[dict_size], "\n")] = '\0';
        for (char *p = dict[dict_size]; *p; p++) *p = toupper(*p);
        dict_size++;
    }
    fclose(f);
}

int word_in_dict(const char *w) {
    int lo = 0, hi = dict_size - 1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        int cmp = strcmp(dict[mid], w);
        if (cmp == 0) return 1;
        if (cmp < 0) lo = mid + 1; else hi = mid - 1;
    }
    return 0;
}
```

**Screen refresh modes** — `FullUpdate()` is slow but clean,
`PartialUpdate(x,y,w,h)` is fast but may ghost. Use partial for
frequently-changing areas (grid, HUD), full for screen transitions.

---

## Links

- SDK & examples: https://github.com/pmartin/pocketbook-demo
- inkview.h headers: https://github.com/blchinezu/pocketbook-sdk
- French 4x4 dice letter layout source: http://baggle.org/regles.php
- French 5x5 ("Big B@ggle") dice letter layout source: http://baggle.org/regles.php
- English 4x4 dice, researched from
  https://boardgames.stackexchange.com/q/48151 and cross-checked
  against several independent published transcriptions
- English 5x5 ("Big Boggle") dice, physically verified against an
  owned 1979 set: https://en.wikipedia.org/wiki/Talk:Boggle
- Big Boggle's 4-letter minimum word length, confirmed by
  UltraBoardGames' rules page and by baggle.org's own Big B@ggle rules
  text — see `docs/board_size_5x5_plan.md` for the full citation trail
