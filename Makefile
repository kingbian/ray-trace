# Compiler
CC = gcc

# Compiler Flags
CFLAGS = -g -Wall

# Libraries
LIBS = -lSDL2 -lm

# Source files
SRCS = main.c ray-trace.c

# Executable file
OUT = main.exe

# Default target
all: $(OUT)

# Compile the executable
$(OUT): $(SRCS)
	$(CC) $(CFLAGS) -o $(OUT) $(SRCS) $(LIBS)

# Run the program
run: all
	./$(OUT)

# Clean compiled files
clean:
	rm -f $(OUT)

