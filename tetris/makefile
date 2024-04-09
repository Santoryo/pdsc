all: tetris

tetris: tetris.o primlib.o tetrominos.o
	gcc -fsanitize=undefined -g $^ -o $@  -lSDL2_gfx `sdl2-config --libs` -lm

.c.o: 
	gcc -fsanitize=undefined  -g -Wall -pedantic `sdl2-config --cflags` -c  $<

primlib.o: primlib.c primlib.h

tetrominos.o: tetrominos.c tetrominos.h

tetris.o: tetris.c primlib.h tetrominos.h

clean:
	-rm primlib.o tetris.o tetrominos.o tetris