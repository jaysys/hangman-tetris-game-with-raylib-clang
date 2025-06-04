# Platform detection for different operating systems
UNAME_S := $(shell uname -s)

# Compiler
CC = gcc

# Detect OS
UNAME_S := $(shell uname -s)

# Compiler flags
CFLAGS = -Wall -std=c99 -D_DEFAULT_SOURCE -Wno-missing-braces -Wunused-result
INCLUDES = -I/opt/homebrew/include -Isrc/hangman -Isrc/tetris

# Platform-specific settings
ifeq ($(UNAME_S),Darwin)
    # macOS
    LDFLAGS = -L/opt/homebrew/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
else
    # Linux
    LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
endif

# Source files
SRC = main.c \
      src/hangman/hangman.c \
      src/tetris/tetris.c

OBJ = $(SRC:.c=.o)

# Target
TARGET = raylib_app

# Build rules
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ)
	find . -name "*.o" -delete

.PHONY: all clean cleanall

# Clean all build artifacts including the final executable
cleanall: clean
	rm -f $(TARGET)

# Clean only intermediate object files
clean:
	rm -f $(OBJ)
	find . -name "*.o" -delete
