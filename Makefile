CC = gcc
CFLAGS = -std=c17 -Wall -Wextra `pkg-config --cflags gtk+-3.0`
LDFLAGS = `pkg-config --libs gtk+-3.0`

TARGET = Minesweeper
MAIN = main.c
SRCS = logic/board.c GUI/sprites/sprite_sheets.c $(MAIN)  # Include main.c here
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

run: $(TARGET)
	./$(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)
