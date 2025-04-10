# Compiler and flags
CC = g++
CFLAGS = -Wall 

# Source files and target
SRC = src/main.c 
OBJ = $(SRC:.c=.o)
TARGET = client

# Default rule
all: $(TARGET)

# Linking
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Object file compilation
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJ) $(TARGET)

# Phony targets
.PHONY: all clean


