#include "hangman.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

// Game variables
static const char* words[] = {
    "RAYLIB", "PROGRAMMING", "HANGMAN", "COMPUTER", "KEYBOARD",
    "DEVELOPER", "SOFTWARE", "VARIABLE", "FUNCTION", "POINTER"
};
static const int wordsCount = 10;

// Screen width for drawing
static const int screenWidth = 800;

void PlayHangman(void) {
    // Game variables
    char secretWord[50] = {0};
    char guessedWord[50] = {0};
    char usedLetters[26] = {0};
    int usedCount = 0;
    int mistakes = 0;
    const int maxMistakes = 6;
    HangmanGameState gameState = GAME_PLAYING;
    
    // Select a random word
    srand((unsigned int)time(NULL));
    strcpy(secretWord, words[rand() % wordsCount]);
    
    // Initialize guessed word with underscores
    int len = (int)strlen(secretWord);
    for (int i = 0; i < len; i++)
        guessedWord[i] = (secretWord[i] == ' ') ? ' ' : '_';
    guessedWord[len] = '\0';
    
    // Game loop
    while (!WindowShouldClose()) {
        // Check for exit
        if (IsKeyPressed(KEY_ESCAPE)) {
            break;
        }
        
        // Update
        if (gameState == GAME_PLAYING) {
            // Check for letter input
            int key = GetKeyPressed();
            if (key == KEY_ESCAPE) {
                break;
            }
            
            if (key >= 'A' && key <= 'Z') key += 32; // Convert to lowercase
            
            if (key >= 'a' && key <= 'z') {
                // Check if letter was already used
                int alreadyUsed = 0;
                for (int i = 0; i < usedCount; i++) {
                    if (usedLetters[i] == key) {
                        alreadyUsed = 1;
                        break;
                    }
                }
                
                if (!alreadyUsed) {
                    usedLetters[usedCount++] = key;
                    
                    // Check if letter is in the word
                    int found = 0;
                    for (int i = 0; i < len; i++) {
                        if (tolower(secretWord[i]) == key) {
                            guessedWord[i] = secretWord[i];
                            found = 1;
                        }
                    }
                    
                    if (!found) mistakes++;
                    
                    // Check win condition
                    if (strcmp(secretWord, guessedWord) == 0) {
                        gameState = GAME_WON;
                    }
                    // Check lose condition
                    else if (mistakes >= maxMistakes) {
                        gameState = GAME_LOST;
                    }
                }
            }
        }
        else if (IsKeyPressed(KEY_ENTER)) {
            break;
        }
        
        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        // Draw hangman
        DrawRectangle(screenWidth/2 - 100, 100, 200, 20, BROWN);
        DrawRectangle(screenWidth/2, 120, 20, 200, BROWN);
        DrawRectangle(screenWidth/2 - 100, 120, 20, 30, BROWN);
        
        if (mistakes > 0) DrawCircle(screenWidth/2 - 90, 175, 25, GRAY); // Head
        if (mistakes > 1) DrawLine(screenWidth/2 - 90, 200, screenWidth/2 - 90, 250, GRAY); // Body
        if (mistakes > 2) DrawLine(screenWidth/2 - 90, 210, screenWidth/2 - 120, 240, GRAY); // Left arm
        if (mistakes > 3) DrawLine(screenWidth/2 - 90, 210, screenWidth/2 - 60, 240, GRAY); // Right arm
        if (mistakes > 4) DrawLine(screenWidth/2 - 90, 250, screenWidth/2 - 120, 290, GRAY); // Left leg
        if (mistakes > 5) DrawLine(screenWidth/2 - 90, 250, screenWidth/2 - 60, 290, GRAY); // Right leg
        
        // Draw word to guess
        int wordWidth = MeasureText(guessedWord, 40);
        DrawText(guessedWord, screenWidth/2 - wordWidth/2, 350, 40, BLACK);
        
        // Draw used letters
        if (usedCount > 0) {
            char usedText[50] = "Used letters: ";
            strcat(usedText, usedLetters);
            DrawText(usedText, 20, 450, 20, GRAY);
        }
        
        // Draw game over or win message
        if (gameState == GAME_LOST) {
            DrawText("GAME OVER!", screenWidth/2 - 100, 400, 30, RED);
            DrawText(TextFormat("The word was: %s", secretWord), screenWidth/2 - 150, 430, 20, DARKGRAY);
            DrawText("Press ENTER to return to menu", screenWidth/2 - 180, 460, 20, DARKGRAY);
        }
        else if (gameState == GAME_WON) {
            DrawText("YOU WIN!", screenWidth/2 - 80, 400, 30, GREEN);
            DrawText("Press ENTER to return to menu", screenWidth/2 - 180, 430, 20, DARKGRAY);
        }
        
        EndDrawing();
    }
}
