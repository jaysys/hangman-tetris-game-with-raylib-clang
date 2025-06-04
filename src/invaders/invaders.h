#ifndef INVADERS_H
#define INVADERS_H

#include "raylib.h"

// Game states
typedef enum {
    INVADERS_TITLE,
    INVADERS_PLAYING,
    INVADERS_GAME_OVER
} InvadersGameState;

// Player structure
typedef struct {
    Vector2 position;
    int width;
    int height;
    int speed;
    bool alive;
} Player;

// Bullet structure
typedef struct {
    Vector2 position;
    int speed;
    bool active;
    int width;
    int height;
} Bullet;

// Invader structure
typedef struct {
    Vector2 position;
    bool alive;
    Color color;
    int points;
} Invader;

// Game structure
typedef struct {
    Player player;
    Bullet bullets[10];
    Invader invaders[55]; // 11 columns x 5 rows
    int score;
    int lives;
    InvadersGameState state;
    int invaderDirection;
    float invaderMoveTimer;
    float invaderMoveInterval;
    float bulletCooldown;
} Game;

// Function declarations
void InitGame(Game *game);
void UpdateGame(Game *game);
void DrawGame(Game *game);
void ResetGame(Game *game);
void FireBullet(Game *game);
void UpdateBullets(Game *game);
void UpdateInvaders(Game *game);
void CheckCollisions(Game *game);
void DrawTitleScreen(void);
void DrawGameOverScreen(int score);
void PlayInvaders(void);

#endif // INVADERS_H
