#include "invaders.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// Screen dimensions
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// Game constants
#define PLAYER_WIDTH 60
#define PLAYER_HEIGHT 20
#define BULLET_WIDTH 4
#define BULLET_HEIGHT 15
#define INVADER_ROWS 5
#define INVADER_COLS 11
#define INVADER_WIDTH 40
#define INVADER_HEIGHT 30
#define INVADER_PADDING 10

// Initialize game
void InitGame(Game *game) {
    // Initialize player
    game->player = (Player){
        .position = (Vector2){SCREEN_WIDTH/2 - PLAYER_WIDTH/2, SCREEN_HEIGHT - 50},
        .width = PLAYER_WIDTH,
        .height = PLAYER_HEIGHT,
        .speed = 5,
        .alive = true
    };
    
    // Initialize bullets
    for (int i = 0; i < 10; i++) {
        game->bullets[i] = (Bullet){
            .position = (Vector2){0, 0},
            .speed = 7,
            .active = false,
            .width = BULLET_WIDTH,
            .height = BULLET_HEIGHT
        };
    }
    
    // Initialize invaders
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 11; col++) {
            int index = row * 11 + col;
            game->invaders[index].position = (Vector2){
                100 + col * (INVADER_WIDTH + INVADER_PADDING),
                50 + row * (INVADER_HEIGHT + INVADER_PADDING)
            };
            game->invaders[index].alive = true;
            
            // Different colors and points for different rows
            if (row == 0) {
                game->invaders[index].color = RED;
                game->invaders[index].points = 30;
            } else if (row < 3) {
                game->invaders[index].color = PINK;
                game->invaders[index].points = 20;
            } else {
                game->invaders[index].color = GREEN;
                game->invaders[index].points = 10;
            }
        }
    }
    
    // Initialize game state
    game->score = 0;
    game->lives = 3;
    game->state = INVADERS_TITLE;
    game->invaderDirection = 1;
    game->invaderMoveTimer = 0.0f;
    game->invaderMoveInterval = 0.5f;
    game->bulletCooldown = 0.0f;
}

// Reset game
void ResetGame(Game *game) {
    InitGame(game);
}

// Fire a bullet
void FireBullet(Game *game) {
    if (game->bulletCooldown <= 0) {
        for (int i = 0; i < 10; i++) {
            if (!game->bullets[i].active) {
                game->bullets[i].position = (Vector2){
                    game->player.position.x + game->player.width/2 - BULLET_WIDTH/2,
                    game->player.position.y - BULLET_HEIGHT
                };
                game->bullets[i].active = true;
                game->bulletCooldown = 0.3f; // Cooldown in seconds
                break;
            }
        }
    }
}

// Update bullets
void UpdateBullets(Game *game) {
    for (int i = 0; i < 10; i++) {
        if (game->bullets[i].active) {
            game->bullets[i].position.y -= game->bullets[i].speed;
            
            // Deactivate bullet if it goes off screen
            if (game->bullets[i].position.y < 0) {
                game->bullets[i].active = false;
            }
        }
    }
    
    // Update bullet cooldown
    if (game->bulletCooldown > 0) {
        game->bulletCooldown -= GetFrameTime();
    }
}

// Update invaders
void UpdateInvaders(Game *game) {
    game->invaderMoveTimer += GetFrameTime();
    
    if (game->invaderMoveTimer >= game->invaderMoveInterval) {
        game->invaderMoveTimer = 0.0f;
        
        bool moveDown = false;
        float minX = SCREEN_WIDTH;
        float maxX = 0;
        float maxY = 0;
        
        // Find the boundaries of the invaders
        for (int i = 0; i < 55; i++) {
            if (game->invaders[i].alive) {
                if (game->invaders[i].position.x < minX) minX = game->invaders[i].position.x;
                if (game->invaders[i].position.x > maxX) maxX = game->invaders[i].position.x + INVADER_WIDTH;
                if (game->invaders[i].position.y > maxY) maxY = game->invaders[i].position.y;
                
                // Check if any invader has reached the sides
                if ((game->invaders[i].position.x <= 10 && game->invaderDirection < 0) ||
                    (game->invaders[i].position.x + INVADER_WIDTH >= SCREEN_WIDTH - 10 && game->invaderDirection > 0)) {
                    moveDown = true;
                }
            }
        }
        
        // Move invaders down and reverse direction if needed
        if (moveDown) {
            game->invaderDirection *= -1;
            for (int i = 0; i < 55; i++) {
                if (game->invaders[i].alive) {
                    game->invaders[i].position.y += 10;
                }
            }
        } else {
            // Move invaders horizontally
            for (int i = 0; i < 55; i++) {
                if (game->invaders[i].alive) {
                    game->invaders[i].position.x += 10 * game->invaderDirection;
                }
            }
        }
        
        // Check if invaders reached the bottom
        if (maxY + INVADER_HEIGHT >= game->player.position.y) {
            game->state = INVADERS_GAME_OVER;
        }
    }
}

// Check collisions
void CheckCollisions(Game *game) {
    // Check bullet-invader collisions
    for (int b = 0; b < 10; b++) {
        if (game->bullets[b].active) {
            for (int i = 0; i < 55; i++) {
                if (game->invaders[i].alive &&
                    game->bullets[b].position.x < game->invaders[i].position.x + INVADER_WIDTH &&
                    game->bullets[b].position.x + BULLET_WIDTH > game->invaders[i].position.x &&
                    game->bullets[b].position.y < game->invaders[i].position.y + INVADER_HEIGHT &&
                    game->bullets[b].position.y + BULLET_HEIGHT > game->invaders[i].position.y) {
                    
                    // Hit an invader
                    game->invaders[i].alive = false;
                    game->bullets[b].active = false;
                    game->score += game->invaders[i].points;
                    
                    // Check if all invaders are dead
                    bool allDead = true;
                    for (int j = 0; j < 55; j++) {
                        if (game->invaders[j].alive) {
                            allDead = false;
                            break;
                        }
                    }
                    
                    if (allDead) {
                        // Level complete, reset with faster invaders
                        ResetGame(game);
                        game->invaderMoveInterval = fmax(0.2f, game->invaderMoveInterval - 0.05f);
                    }
                    
                    break;
                }
            }
        }
    }
}

// Draw title screen
void DrawTitleScreen(void) {
    DrawText("SPACE INVADERS", SCREEN_WIDTH/2 - MeasureText("SPACE INVADERS", 50)/2, 150, 50, WHITE);
    DrawText("Press ENTER to Start", SCREEN_WIDTH/2 - MeasureText("Press ENTER to Start", 30)/2, 300, 30, WHITE);
    DrawText("ARROW KEYS to Move, SPACE to Shoot", SCREEN_WIDTH/2 - MeasureText("ARROW KEYS to Move, SPACE to Shoot", 20)/2, 350, 20, WHITE);
}

// Draw game over screen
void DrawGameOverScreen(int score) {
    char scoreText[50];
    sprintf(scoreText, "GAME OVER - SCORE: %d", score);
    DrawText(scoreText, SCREEN_WIDTH/2 - MeasureText(scoreText, 40)/2, 200, 40, WHITE);
    DrawText("Press ENTER to Play Again", SCREEN_WIDTH/2 - MeasureText("Press ENTER to Play Again", 30)/2, 300, 30, WHITE);
    DrawText("Press ESC to Return to Menu", SCREEN_WIDTH/2 - MeasureText("Press ESC to Return to Menu", 20)/2, 350, 20, WHITE);
}

// Draw game
void DrawGame(Game *game) {
    // Draw player
    DrawRectangleRec((Rectangle){game->player.position.x, game->player.position.y, 
                               (float)game->player.width, (float)game->player.height}, WHITE);
    
    // Draw bullets
    for (int i = 0; i < 10; i++) {
        if (game->bullets[i].active) {
            DrawRectangleRec((Rectangle){game->bullets[i].position.x, game->bullets[i].position.y,
                                       (float)game->bullets[i].width, (float)game->bullets[i].height}, GREEN);
        }
    }
    
    // Draw invaders
    for (int i = 0; i < 55; i++) {
        if (game->invaders[i].alive) {
            DrawRectangleRec((Rectangle){game->invaders[i].position.x, game->invaders[i].position.y,
                                       INVADER_WIDTH, INVADER_HEIGHT}, game->invaders[i].color);
        }
    }
    
    // Draw score and lives
    char scoreText[20];
    sprintf(scoreText, "SCORE: %d", game->score);
    DrawText(scoreText, 20, 20, 20, WHITE);
    
    char livesText[20];
    sprintf(livesText, "LIVES: %d", game->lives);
    DrawText(livesText, SCREEN_WIDTH - 120, 20, 20, WHITE);
}

// Update game
void UpdateGame(Game *game) {
    if (IsKeyPressed(KEY_ENTER)) {
        if (game->state == INVADERS_TITLE) {
            game->state = INVADERS_PLAYING;
        } else if (game->state == INVADERS_GAME_OVER) {
            ResetGame(game);
            game->state = INVADERS_PLAYING;
        }
    }
    
    if (game->state != INVADERS_PLAYING) return;
    
    // Player movement
    if (IsKeyDown(KEY_LEFT) && game->player.position.x > 0) {
        game->player.position.x -= game->player.speed;
    }
    if (IsKeyDown(KEY_RIGHT) && game->player.position.x < SCREEN_WIDTH - game->player.width) {
        game->player.position.x += game->player.speed;
    }
    
    // Shooting
    if (IsKeyDown(KEY_SPACE)) {
        FireBullet(game);
    }
    
    UpdateBullets(game);
    UpdateInvaders(game);
    CheckCollisions(game);
}

// Main game function
void PlayInvaders(void) {
    Game game;
    InitGame(&game);
    
    while (!WindowShouldClose()) {
        // Update
        UpdateGame(&game);
        
        // Draw
        BeginDrawing();
        ClearBackground(BLACK);
        
        if (game.state == INVADERS_TITLE) {
            DrawTitleScreen();
        } else if (game.state == INVADERS_GAME_OVER) {
            DrawGameOverScreen(game.score);
        } else {
            DrawGame(&game);
        }
        
        EndDrawing();
        
        // Check for ESC to return to menu
        if (IsKeyPressed(KEY_ESCAPE) && game.state == INVADERS_GAME_OVER) {
            break;
        }
    }
}
