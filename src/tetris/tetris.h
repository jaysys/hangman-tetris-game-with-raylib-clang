#ifndef TETRIS_H
#define TETRIS_H

#include "raylib.h"

// Tetromino types
typedef enum {
    TETRO_EMPTY = 0,
    TETRO_CYAN,
    TETRO_BLUE,
    TETRO_ORANGE,
    TETRO_YELLOW,
    TETRO_GREEN,
    TETRO_PURPLE,
    TETRO_RED
} TetrominoType;

typedef struct {
    int grid[20][10];
    int currentPiece[4][4];
    int nextPiece[4][4];
    int pieceX, pieceY;
    int nextPieceType;
    int currentPieceType;
    int rotation;
    float fallTimer;
    float fallSpeed;
    int score;
    int level;
    int linesCleared;
    bool gameOver;
} TetrisGame;

// Function declarations
void InitTetrisGame(TetrisGame *game);
void UpdateTetrisGame(TetrisGame *game);
void DrawTetrisGame(const TetrisGame *game);
void PlayTetris(void);

#endif // TETRIS_H
