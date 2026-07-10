/* English UI strings. See i18n.h. */

#include "i18n.h"

const LangEntry I18N_EN[] = {
    { STR_CHOOSE_MODE,          "Set up your game" },
    { STR_BTN_SOLO,              "Solo" },
    { STR_SOLO_HINT,             "Tap cells to spell words" },
    { STR_BTN_MULTIPLAYER,       "Multiplayer" },
    { STR_MULTI_HINT,            "Compare word lists at the end" },
    { STR_LABEL_BOARD_SIZE,      "Grid size" },
    { STR_LABEL_TIMER,           "Timer" },
    { STR_BTN_START,             "Start" },

    { STR_HUD_TITLE_SOLO,        "BOOKGGLE Solo" },
    { STR_HUD_TITLE_MULTI,       "BOOKGGLE Multi" },
    { STR_SCORE_LABEL,           "Score: %d" },   /* keep the %d placeholder */
    { STR_BTN_MODE,              "Mode" },
    { STR_BTN_QUIT,              "Quit" },
    { STR_BTN_PAUSE,             "Pause" },
    { STR_BTN_PLAY,              "Play" },
    { STR_BTN_SUBMIT,            "Submit" },
    { STR_BTN_CLEAR,             "Clear" },
    { STR_WORDS_HEADER,          "Words (%d)" },  /* keep the %d placeholder */
    { STR_MULTI_INGAME_HINT,     "Note your words - grid disappears at 0:00" },

    { STR_TIMES_UP,              "TIME'S UP!" },
    { STR_COMPARE_LISTS,         "Compare your word lists." },
    { STR_CANCEL_RULE,           "Words found by more than one player cancel out." },
    { STR_BTN_NEW_GAME,          "New game" },
    { STR_BTN_CHANGE_MODE,       "Change mode" },

    { STR_GAME_OVER_TITLE,       "Game over" },
    { STR_SCORE_FINAL_SINGULAR,  "Time's up!\nFinal score: %d point" },
    { STR_SCORE_FINAL_PLURAL,    "Time's up!\nFinal score: %d points" },

    { STR_TOO_SHORT_TITLE,       "Too short" },
    { STR_TOO_SHORT_BODY,        "Word must be at least %d letters." },  /* keep the %d placeholder */
    { STR_ALREADY_FOUND_TITLE,   "Already found" },
    { STR_ALREADY_FOUND_BODY,    "This word was already validated!" },
    { STR_INVALID_TITLE,         "Invalid word" },
    { STR_INVALID_BODY,          "This word is not in the dictionary." },

    { STR_LANG_CODE,             "en" },
};

const int I18N_EN_COUNT = sizeof(I18N_EN) / sizeof(I18N_EN[0]);
