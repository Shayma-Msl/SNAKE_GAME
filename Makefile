CC = gcc
CFLAGS = -Wall -Wextra -DSDL_TTF_OK `sdl-config --cflags`
LIBS = `sdl-config --libs` -lm -lSDL_ttf

OBJ = main.o map.o snake.o game.o display.o graphics/graphics.o

snake: $(OBJ)
	$(CC) $(OBJ) -o snake $(LIBS)

main.o: main.c map.h snake.h game.h display.h
	$(CC) $(CFLAGS) -c main.c

map.o: map.c map.h
	$(CC) $(CFLAGS) -c map.c

snake.o: snake.c snake.h
	$(CC) $(CFLAGS) -c snake.c

game.o: game.c game.h map.h snake.h
	$(CC) $(CFLAGS) -c game.c

display.o: display.c display.h map.h snake.h game.h graphics/graphics.h
	$(CC) $(CFLAGS) -c display.c

graphics/graphics.o: graphics/graphics.c graphics/graphics.h
	$(CC) $(CFLAGS) -c graphics/graphics.c -o graphics/graphics.o

clean:
	rm -f $(OBJ) snake
