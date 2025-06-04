# Hangman and Tetris Game with Raylib C language

A collection of classic games built with raylib, featuring Hangman and Tetris. This project demonstrates game development using C and raylib with a clean, modular architecture.

## 🎮 Games Included

### 1. Hangman

A word guessing game where you try to guess the hidden word before running out of attempts.

### 2. Tetris

The classic tile-matching puzzle game where you arrange falling tetrominoes to complete lines.

## 📋 Prerequisites

- C compiler (gcc, clang, or MSVC)
- raylib library (version 5.5 or later)
- GNU Make

## 🛠️ Installation

### macOS

```bash
# Install raylib using Homebrew
brew install raylib

# Clone the repository
git clone https://github.com/yourusername/raylib-game-collection.git
cd raylib-game-collection
```

### Linux (Ubuntu/Debian)

```bash
# Install dependencies
sudo apt update
sudo apt install build-essential git libraylib-dev

# Clone the repository
git clone https://github.com/yourusername/raylib-game-collection.git
cd raylib-game-collection
```

### Windows

1. Install MSYS2 from https://www.msys2.org/
2. Open MSYS2 MINGW64 terminal and run:
   ```bash
   pacman -Syu
   pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-raylib make
   ```
3. Clone the repository and navigate to it

## 🏗️ Building and Compilation

### Prerequisites

- C compiler (gcc/clang on Unix-like systems, MSVC on Windows)
- raylib development libraries
- GNU Make

### Build Commands

#### Build the project

```bash
make
```

#### Clean object files

```bash
make clean
```

#### Clean everything (including executable)

```bash
make cleanall
```

### Build Options

You can modify the `Makefile` to customize build options:

- `CC`: C compiler (default: gcc)
- `CFLAGS`: Compiler flags
- `LDFLAGS`: Linker flags
- `INCLUDES`: Additional include paths

Example of custom build:

```bash
make CFLAGS="-O2 -Wall -std=c99"
```

## 🎮 Running

```bash
./raylib_app
```

## 🕹️ Controls

### Main Menu

- UP/DOWN: Navigate menu
- ENTER: Select game
- ESC: Exit application

### Hangman

- A-Z: Guess a letter
- ENTER: Return to main menu
- ESC: Return to main menu

### Tetris

- LEFT/RIGHT: Move piece horizontally
- UP: Rotate piece
- DOWN: Soft drop
- SPACE: Hard drop (instantly drops the piece)
- ENTER: Restart game (when game over)
- ESC: Return to main menu

## 🏗️ Project Structure and Source Code

### Directory Structure

```
raylib-game-collection/
├── src/
│   ├── hangman/       # Hangman game source files
│   │   ├── hangman.c  # Game logic and rendering
│   │   └── hangman.h  # Game definitions and declarations
│   ├── tetris/        # Tetris game source files
│   │   ├── tetris.c   # Game logic and rendering
│   │   └── tetris.h   # Game definitions and structures
│   └── main.c         # Main application and menu
├── Makefile           # Build configuration
└── README.md          # This file
```

### Source Code Overview

#### Main Application (`main.c`)

- Handles window creation and main game loop
- Implements the main menu system
- Manages game state transitions

#### Hangman Game (`src/hangman/`)

- **hangman.h**: Defines game state and function prototypes
- **hangman.c**: Implements game logic including:
  - Word selection and management
  - User input handling
  - Game state updates
  - Drawing functions using raylib

#### Tetris Game (`src/tetris/`)

- **tetris.h**: Defines game structures and function prototypes

  - `TetrominoType`: Enumerates different tetromino shapes
  - `TetrisGame`: Main game state structure
  - Function declarations for game logic and rendering

- **tetris.c**: Implements Tetris game mechanics:
  - Piece movement and rotation
  - Collision detection
  - Line clearing and scoring
  - Rendering the game board and pieces
  ```c
  // Example of rendering a tetris piece
  void DrawTetrisGame(const TetrisGame *game) {
      const int cellSize = 30;
      // Drawing logic for the game grid and pieces
  }
  ```

### Key Components

1. **Game State Management**

   - Each game maintains its own state structure
   - Clean separation between game logic and rendering

2. **Input Handling**

   - Uses raylib's input system
   - Separate handling for menu and in-game controls

3. **Rendering**
   - Utilizes raylib's 2D rendering capabilities
   - Clean separation of game logic and rendering code
   - Simple but effective visual feedback for game states

### Build System

The project uses a `Makefile` with the following features:

- Automatic dependency tracking
- Platform-specific settings for macOS and Linux
- Multiple build targets for different needs
- Clean separation of source and build artifacts

Build process flow:

1. Compile each source file to object files
2. Link object files with raylib to create the final executable
3. Handle platform-specific library linking

### Adding New Games

1. Create a new directory under `src/` for your game
2. Implement game logic in `.c` and `.h` files
3. Add your source files to the `SRC` variable in the Makefile
4. Update the main menu to include your game

### Debugging

For debugging, you can add compiler flags:

```bash
make CFLAGS="-g -O0"
```

Then use gdb or lldb to debug the application.

## 🚀 Features

### Hangman

- Random word selection
- Visual hangman drawing
- Letter tracking
- Win/lose conditions

### Tetris

- Classic Tetris gameplay
- Next piece preview
- Score and level system
- Line clearing mechanics
- Game over detection

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- [raylib](https://www.raylib.com/) - A simple and easy-to-use library to enjoy videogames programming
- All contributors and the raylib community
