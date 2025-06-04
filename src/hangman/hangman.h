#ifndef HANGMAN_H
#define HANGMAN_H

#include "raylib.h"

typedef enum {
    GAME_PLAYING,
    GAME_WON,
    GAME_LOST
} HangmanGameState;

void PlayHangman(void);

#endif // HANGMAN_H
