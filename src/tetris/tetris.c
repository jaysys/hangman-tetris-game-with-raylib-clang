#include "tetris.h"
#include "raylib.h"
#include <stdlib.h>
#include <time.h>

// Tetromino shapes
static const int tetrominoes[7][4][4] = {
    // I
    {
        {TETRO_EMPTY, TETRO_EMPTY, TETRO_EMPTY, TETRO_EMPTY},
        {TETRO_CYAN, TETRO_CYAN, TETRO_CYAN, TETRO_CYAN},
        {TETRO_EMPTY, TETRO_EMPTY, TETRO_EMPTY, TETRO_EMPTY},
        {TETRO_EMPTY, TETRO_EMPTY, TETRO_EMPTY, TETRO_EMPTY}
    },
    // J
    {
        {TETRO_BLUE, TETRO_EMPTY, TETRO_EMPTY},
        {TETRO_BLUE, TETRO_BLUE, TETRO_BLUE},
        {TETRO_EMPTY, TETRO_EMPTY, TETRO_EMPTY}
    },
    // L
    {
        {TETRO_EMPTY, TETRO_EMPTY, TETRO_ORANGE},
        {TETRO_ORANGE, TETRO_ORANGE, TETRO_ORANGE},
        {TETRO_EMPTY, TETRO_EMPTY, TETRO_EMPTY}
    },
    // O
    {
        {TETRO_YELLOW, TETRO_YELLOW},
        {TETRO_YELLOW, TETRO_YELLOW}
    },
    // S
    {
        {TETRO_EMPTY, TETRO_GREEN, TETRO_GREEN},
        {TETRO_GREEN, TETRO_GREEN, TETRO_EMPTY},
        {TETRO_EMPTY, TETRO_EMPTY, TETRO_EMPTY}
    },
    // T
    {
        {TETRO_EMPTY, TETRO_PURPLE, TETRO_EMPTY},
        {TETRO_PURPLE, TETRO_PURPLE, TETRO_PURPLE},
        {TETRO_EMPTY, TETRO_EMPTY, TETRO_EMPTY}
    },
    // Z
    {
        {TETRO_RED, TETRO_RED, TETRO_EMPTY},
        {TETRO_EMPTY, TETRO_RED, TETRO_RED},
        {TETRO_EMPTY, TETRO_EMPTY, TETRO_EMPTY}
    }
};

static const Color tetrominoColors[] = {
    {0, 0, 0, 0},        // TETRO_EMPTY
    {0, 255, 255, 255},   // TETRO_CYAN (I)
    {0, 0, 255, 255},     // TETRO_BLUE (J)
    {255, 165, 0, 255},   // TETRO_ORANGE (L)
    {255, 255, 0, 255},   // TETRO_YELLOW (O)
    {0, 255, 0, 255},     // TETRO_GREEN (S)
    {128, 0, 128, 255},   // TETRO_PURPLE (T)
    {255, 0, 0, 255}      // TETRO_RED (Z)
};

void InitTetrisGame(TetrisGame *game) {
    // Initialize grid
    for (int y = 0; y < 20; y++) {
        for (int x = 0; x < 10; x++) {
            game->grid[y][x] = TETRO_EMPTY;
        }
    }
    
    // Initialize game state
    game->pieceX = 3;
    game->pieceY = 0;
    game->rotation = 0;
    game->fallSpeed = 1.0f;
    game->score = 0;
    game->level = 1;
    game->linesCleared = 0;
    game->gameOver = false;
    
    // Initialize random seed
    srand(time(NULL));
    
    // Get first pieces
    game->currentPieceType = rand() % 7 + TETRO_CYAN;
    game->nextPieceType = rand() % 7 + TETRO_CYAN;
    
    // Copy current piece
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            game->currentPiece[y][x] = tetrominoes[game->currentPieceType - TETRO_CYAN][y][x];
        }
    }
    // Set initial position
    game->pieceX = 3;
    game->pieceY = 0;
}

bool CheckCollision(TetrisGame *game, int offsetX, int offsetY) {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (game->currentPiece[y][x] != TETRO_EMPTY) {
                int newX = game->pieceX + x + offsetX;
                int newY = game->pieceY + y + offsetY;
                
                if (newX < 0 || newX >= 10 || newY >= 20 || 
                    (newY >= 0 && game->grid[newY][newX] != TETRO_EMPTY)) {
                    return true;
                }
            }
        }
    }
    return false;
}

void LockPiece(TetrisGame *game) {
    // Add piece to grid
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (game->currentPiece[y][x] != TETRO_EMPTY) {
                int gridY = game->pieceY + y;
                int gridX = game->pieceX + x;
                if (gridY >= 0 && gridX >= 0 && gridX < 10) {
                    game->grid[gridY][gridX] = game->currentPieceType;
                }
            }
        }
    }
    
    // Check for completed lines
    int linesCleared = 0;
    for (int y = 19; y >= 0; y--) {
        bool lineComplete = true;
        for (int x = 0; x < 10; x++) {
            if (game->grid[y][x] == TETRO_EMPTY) {
                lineComplete = false;
                break;
            }
        }
        
        if (lineComplete) {
            // Remove line and move everything down
            for (int ny = y; ny > 0; ny--) {
                for (int x = 0; x < 10; x++) {
                    game->grid[ny][x] = game->grid[ny-1][x];
                }
            }
            // Clear top line
            for (int x = 0; x < 10; x++) {
                game->grid[0][x] = TETRO_EMPTY;
            }
            
            linesCleared++;
            y++; // Check the same row again
        }
    }
    
    // Update score
    if (linesCleared > 0) {
        int points = 0;
        switch (linesCleared) {
            case 1: points = 100; break;
            case 2: points = 300; break;
            case 3: points = 500; break;
            case 4: points = 800; break;
        }
        game->score += points * game->level;
        game->linesCleared += linesCleared;
        game->level = game->linesCleared / 10 + 1;
        game->fallSpeed = 0.5f / game->level;
    }
    
    // Get next piece
    game->currentPieceType = game->nextPieceType;
    game->nextPieceType = rand() % 7 + TETRO_CYAN;
    
    // Copy new current piece
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (y < 3 && x < 3) {
                game->currentPiece[y][x] = tetrominoes[game->currentPieceType - TETRO_CYAN][y][x];
            } else {
                game->currentPiece[y][x] = TETRO_EMPTY;
            }
        }
    }
    
    // Reset position
    game->pieceX = 3;
    game->pieceY = 0;
    
    // Check if game over
    if (CheckCollision(game, 0, 0)) {
        game->gameOver = true;
    }
}

void RotatePiece(TetrisGame *game) {
    int temp[4][4] = {0};
    
    // Create a copy of the current piece
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            temp[y][x] = game->currentPiece[y][x];
        }
    }
    
    // Rotate the piece
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            game->currentPiece[x][3 - y] = temp[y][x];
        }
    }
    
    // If rotation causes collision, rotate back
    if (CheckCollision(game, 0, 0)) {
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++) {
                game->currentPiece[y][x] = temp[y][x];
            }
        }
    }
}

void UpdateTetrisGame(TetrisGame *game) {
    if (game->gameOver) {
        if (IsKeyPressed(KEY_ENTER)) {
            InitTetrisGame(game);
        }
        return;
    }
    
    // Move left
    if (IsKeyPressed(KEY_LEFT) && !CheckCollision(game, -1, 0)) {
        game->pieceX--;
    }
    
    // Move right
    if (IsKeyPressed(KEY_RIGHT) && !CheckCollision(game, 1, 0)) {
        game->pieceX++;
    }
    
    // Rotate
    if (IsKeyPressed(KEY_UP)) {
        RotatePiece(game);
    }
    
    // Soft drop
    if (IsKeyDown(KEY_DOWN)) {
        game->fallTimer += 2 * GetFrameTime();
    }
    
    // Hard drop
    if (IsKeyPressed(KEY_SPACE)) {
        while (!CheckCollision(game, 0, 1)) {
            game->pieceY++;
        }
        LockPiece(game);
        return;
    }
    
    // Fall
    game->fallTimer += GetFrameTime();
    if (game->fallTimer >= game->fallSpeed) {
        game->fallTimer = 0;
        
        if (!CheckCollision(game, 0, 1)) {
            game->pieceY++;
        } else {
            LockPiece(game);
        }
    }
}

void DrawTetrisGame(const TetrisGame *game) {
    const int cellSize = 30;
    const int offsetX = (GetScreenWidth() - 10 * cellSize) / 2;
    const int offsetY = 50;
    
    // Draw grid background
    DrawRectangle(offsetX, offsetY, 10 * cellSize, 20 * cellSize, DARKGRAY);
    
    // Draw grid lines
    for (int x = 0; x <= 10; x++) {
        DrawLine(offsetX + x * cellSize, offsetY, 
                 offsetX + x * cellSize, offsetY + 20 * cellSize, GRAY);
    }
    for (int y = 0; y <= 20; y++) {
        DrawLine(offsetX, offsetY + y * cellSize, 
                 offsetX + 10 * cellSize, offsetY + y * cellSize, GRAY);
    }
    
    // Draw placed pieces
    for (int y = 0; y < 20; y++) {
        for (int x = 0; x < 10; x++) {
            if (game->grid[y][x] != TETRO_EMPTY) {
                DrawRectangle(offsetX + x * cellSize + 1, 
                             offsetY + y * cellSize + 1, 
                             cellSize - 1, cellSize - 1, 
                             tetrominoColors[game->grid[y][x]]);
            }
        }
    }
    
    // Draw current piece
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (game->currentPiece[y][x] != TETRO_EMPTY) {
                int drawX = game->pieceX + x;
                int drawY = game->pieceY + y;
                
                if (drawY >= 0) {
                    DrawRectangle(offsetX + drawX * cellSize + 1, 
                                 offsetY + drawY * cellSize + 1, 
                                 cellSize - 1, cellSize - 1, 
                                 tetrominoColors[game->currentPieceType]);
                }
            }
        }
    }
    
    // Draw next piece preview
    DrawText("NEXT:", offsetX + 12 * cellSize, offsetY, 20, WHITE);
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (y < 3 && x < 3 && tetrominoes[game->nextPieceType - TETRO_CYAN][y][x] != TETRO_EMPTY) {
                DrawRectangle(offsetX + 12 * cellSize + x * cellSize, 
                             offsetY + 30 + y * cellSize, 
                             cellSize - 1, cellSize - 1, 
                             tetrominoColors[game->nextPieceType]);
            }
        }
    }
    
    // Draw score and level
    DrawText(TextFormat("SCORE: %d", game->score), offsetX, 10, 20, WHITE);
    DrawText(TextFormat("LEVEL: %d", game->level), offsetX + 200, 10, 20, WHITE);
    
    // Draw game over message
    if (game->gameOver) {
        DrawRectangle(offsetX, offsetY + 8 * cellSize, 10 * cellSize, 4 * cellSize, Fade(BLACK, 0.8f));
        DrawText("GAME OVER", offsetX + 50, offsetY + 9 * cellSize, 40, RED);
        DrawText("Press ENTER to restart", offsetX + 30, offsetY + 10 * cellSize, 20, WHITE);
    }
}

void PlayTetris(void) {
    // Initialize game
    TetrisGame game;
    InitTetrisGame(&game);
    
    // Game loop
    while (!WindowShouldClose()) {
        // Check for exit
        if (IsKeyPressed(KEY_ESCAPE)) {
            break;
        }
        
        // Update
        UpdateTetrisGame(&game);
        
        // Draw
        BeginDrawing();
        ClearBackground(BLACK);
        
        DrawTetrisGame(&game);
        
        // Draw controls
        DrawText("CONTROLS:", 30, 500, 20, WHITE);
        DrawText("LEFT/RIGHT: Move", 30, 530, 20, WHITE);
        DrawText("UP: Rotate", 30, 550, 20, WHITE);
        DrawText("DOWN: Soft Drop", 30, 570, 20, WHITE);
        DrawText("SPACE: Hard Drop", 30, 590, 20, WHITE);
        DrawText("ESC: Back to Menu", 30, 630, 20, YELLOW);
        
        EndDrawing();
    }
}
