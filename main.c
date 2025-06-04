#include "raylib.h"
#include "src/hangman/hangman.h"
#include "src/tetris/tetris.h"
#include "src/invaders/invaders.h"

// Menu items
typedef enum {
    MENU_HANGMAN,
    MENU_TETRIS,
    MENU_INVADERS,
    MENU_EXIT,
    MENU_ITEMS_COUNT
} MenuItem;

// Main game loop function
void RunGame(void) {
    const int screenWidth = 800;
    const int screenHeight = 600;
    
    // Initialize window
    InitWindow(screenWidth, screenHeight, "Game Collection");
    SetTargetFPS(60);
    
    int selectedItem = 0;
    const char* menuItems[MENU_ITEMS_COUNT] = {
        "Hangman Game",
        "Tetris",
        "Space Invaders",
        "Exit"
    };
    
    // Main game loop
    while (!WindowShouldClose()) {
        // Update
        if (IsKeyPressed(KEY_UP)) {
            selectedItem--;
            if (selectedItem < 0) selectedItem = MENU_ITEMS_COUNT - 1;
        }
        else if (IsKeyPressed(KEY_DOWN)) {
            selectedItem++;
            if (selectedItem >= MENU_ITEMS_COUNT) selectedItem = 0;
        }
        
        if (IsKeyPressed(KEY_ENTER)) {
            switch (selectedItem) {
                case MENU_HANGMAN: {
                    // Create a new window for Hangman
                    int gameWidth = 800;
                    int gameHeight = 600;
                    
                    // Close the menu window
                    CloseWindow();
                    
                    // Initialize Hangman window
                    InitWindow(gameWidth, gameHeight, "Hangman");
                    SetTargetFPS(60);
                    
                    PlayHangman();
                    
                    // After Hangman is done, close its window and reopen menu
                    CloseWindow();
                    InitWindow(screenWidth, screenHeight, "Game Collection");
                    SetTargetFPS(60);
                    break;
                }
                case MENU_TETRIS: {
                    // Create a new window for Tetris
                    int gameWidth = 800;
                    int gameHeight = 700;
                    
                    // Close the menu window
                    CloseWindow();
                    
                    // Initialize Tetris window
                    InitWindow(gameWidth, gameHeight, "Tetris");
                    SetTargetFPS(60);
                    
                    PlayTetris();
                    
                    // After Tetris is done, close its window and reopen menu
                    CloseWindow();
                    InitWindow(screenWidth, screenHeight, "Game Collection");
                    SetTargetFPS(60);
                    break;
                }
                case MENU_INVADERS: {
                    // Create a new window for Space Invaders
                    int gameWidth = 800;
                    int gameHeight = 600;
                    
                    // Close the menu window
                    CloseWindow();
                    
                    // Initialize Space Invaders window
                    InitWindow(gameWidth, gameHeight, "Space Invaders");
                    SetTargetFPS(60);
                    
                    PlayInvaders();
                    
                    // After Space Invaders is done, close its window and reopen menu
                    CloseWindow();
                    InitWindow(screenWidth, screenHeight, "Game Collection");
                    SetTargetFPS(60);
                    break;
                }
                case MENU_EXIT:
                    return;
            }
        }
        
        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        // Draw menu title
        DrawText("GAME COLLECTION", 
                screenWidth/2 - MeasureText("GAME COLLECTION", 40)/2, 
                100, 40, BLACK);
        
        // Draw menu items
        for (int i = 0; i < MENU_ITEMS_COUNT; i++) {
            Color color = (i == selectedItem) ? BLUE : DARKGRAY;
            int textWidth = MeasureText(menuItems[i], 30);
            int yPos = 200 + i * 50;
            
            if (i == selectedItem) {
                DrawText(">", screenWidth/2 - textWidth/2 - 30, yPos, 30, color);
            }
            
            DrawText(menuItems[i], screenWidth/2 - textWidth/2, yPos, 30, color);
        }
        
        // Draw instructions
        DrawText("Use UP/DOWN arrows to navigate, ENTER to select", 
                screenWidth/2 - MeasureText("Use UP/DOWN arrows to navigate, ENTER to select", 20)/2, 
                500, 20, GRAY);
        
        EndDrawing();
    }
    
    CloseWindow();
}

int main(void) {
    // Initialize and run the game
    RunGame();
    return 0;
}
