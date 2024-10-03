CC = gcc
CFLAGS = -std=c17 -Wall -Wextra `pkg-config --cflags gtk+-3.0` -g ## TODO remove -g
LDFLAGS = `pkg-config --libs gtk+-3.0`

TARGET = Minesweeper
MAIN = main.c
SRCS = logic/board.c logic/cell.c logic/game_settings.c GUI/sprites/sprite_sheets.c $(MAIN)
OBJS = $(SRCS:.c=.o)

run: $(TARGET)
	./$(TARGET)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)
