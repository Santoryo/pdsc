all: hanoi

hanoi: hanoi.o primlib.o
	gcc -g $^ -o $@ -fsanitize=undefined -lSDL2_gfx `sdl2-config --libs` -lm

.c.o: 
	gcc -g -Wall -fsanitize=undefined -pedantic `sdl2-config --cflags` -c  $<

primlib.o: primlib.c primlib.h

hanoi.o: hanoi.c primlib.h

clean:
	-rm primlib.o hanoi.o hanoi