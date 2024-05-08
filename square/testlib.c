#include "primlib.h"
#include <stdlib.h>
#include <unistd.h>

#define SQUARE_SIZE 100
#define STEP 10
#define SQUARES_AMOUNT 3

int event;
int currentSquare = 0;

typedef struct {
  int x;
  int y;
} Square;

Square squares[SQUARES_AMOUNT];

void setupSquares()
{
  for(int i = 0; i < SQUARES_AMOUNT; i++)
  {
    squares[i].x = 100;
    squares[i].y = 100;
  }
}

void drawSquare(int idx)
{
  gfx_filledRect(squares[idx].x, squares[idx].y, squares[idx].x + SQUARE_SIZE, squares[idx].y+ SQUARE_SIZE, RED);
}

int main(int argc, char *argv[])
{
  if (gfx_init())
    exit(3);

  while(1)
  {
    event = gfx_pollkey();
		if(event != -1) exit(0);



    // drawSquare(currentSquare);

    gfx_filledRect(0, 0, 1, 1, GREEN);

    gfx_updateScreen();
    SDL_Delay(10);


  }



  gfx_getkey();
  return 0;
}
