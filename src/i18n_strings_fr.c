/* French UI strings. See i18n.h. Unaccented on purpose -- avoids
   depending on accented-glyph coverage in the device's font subset. */

#include "i18n.h"

const LangEntry I18N_FR[] = {
    {STR_CHOOSE_MODE, "Configurez votre partie"},
    {STR_BTN_SOLO, "Solo"},
    {STR_SOLO_HINT, "Touchez les cases pour former des mots"},
    {STR_BTN_MULTIPLAYER, "Multijoueur"},
    {STR_MULTI_HINT, "Comparez les listes de mots a la fin"},
    {STR_LABEL_BOARD_SIZE, "Taille de grille"},
    {STR_LABEL_TIMER, "Minuteur"},
    {STR_BTN_START, "Commencer"},

    {STR_HUD_TITLE_SOLO, "BOOKGGLE Solo"},
    {STR_HUD_TITLE_MULTI, "BOOKGGLE Multi"},
    {STR_SCORE_LABEL, "Score : %d"}, /* keep the %d placeholder */
    {STR_BTN_MODE, "Mode"},
    {STR_BTN_QUIT, "Quitter"},
    {STR_BTN_PAUSE, "Pause"},
    {STR_BTN_PLAY, "Jouer"},
    {STR_BTN_SUBMIT, "Valider"},
    {STR_BTN_CLEAR, "Effacer"},
    {STR_WORDS_HEADER, "Mots (%d)"}, /* keep the %d placeholder */
    {STR_MULTI_INGAME_HINT, "Notez vos mots - la grille disparait a 0:00"},

    {STR_TIMES_UP, "TEMPS ECOULE !"},
    {STR_COMPARE_LISTS, "Comparez vos listes de mots."},
    {STR_CANCEL_RULE, "Les mots trouves par plusieurs joueurs s'annulent."},
    {STR_BTN_NEW_GAME, "Nouvelle partie"},
    {STR_BTN_CHANGE_MODE, "Changer de mode"},

    {STR_GAME_OVER_TITLE, "Partie terminee"},
    {STR_SCORE_FINAL_SINGULAR, "Temps ecoule !\nScore final : %d point"},
    {STR_SCORE_FINAL_PLURAL, "Temps ecoule !\nScore final : %d points"},

    {STR_TOO_SHORT_TITLE, "Trop court"},
    {STR_TOO_SHORT_BODY, "Le mot doit faire au moins %d lettres."}, /* keep the %d placeholder */
    {STR_ALREADY_FOUND_TITLE, "Deja trouve"},
    {STR_ALREADY_FOUND_BODY, "Ce mot a deja ete valide !"},
    {STR_INVALID_TITLE, "Mot invalide"},
    {STR_INVALID_BODY, "Ce mot n'est pas dans le dictionnaire."},

    {STR_LANG_CODE, "fr"},
};

const int I18N_FR_COUNT = sizeof(I18N_FR) / sizeof(I18N_FR[0]);
