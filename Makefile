CC = gcc
CFLAGS = -std=c17 -Wall -Wextra `pkg-config --cflags gtk+-3.0` -g ## TODO remove -g
LDFLAGS = `pkg-config --libs gtk+-3.0`

TARGET = Minesweeper
MAIN = main.c
LOGIC_SRC = logic/board.c logic/cell.c logic/game_difficulties.c logic/timer.c
GUI_SRC = GUI/gui_manager.c GUI/update_display.c GUI/sprites/sprite_sheets.c GUI/difficulty_dialog_box.c
SRCS = $(MAIN) $(LOGIC_SRC) $(GUI_SRC)
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
