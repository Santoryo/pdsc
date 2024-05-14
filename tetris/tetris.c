#include "primlib.h"
#include <stdlib.h>
#include "math.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tetrominos.h"

#define REFRESH_RATE 16.6666
#define COLS 10
#define ROWS 20
#define BLOCK_SIZE 30
#define OFFSET 1
#define EMPTY 0
#define TETROMINO_AMOUNT 7
#define TETROMINO_SIZE 4
#define TETROMINO_ROTATIONS 4
#define SPOT 1
#define PIVOT 2
#define LEVEL_UP_CONDITION 3
#define MAX_SPEED 1
#define DOWN 1
#define LEFT -1
#define RIGHT 1
#define USED_PIECE -1

const int INITIAL_BAG[TETROMINO_AMOUNT] = {0, 1, 2, 3, 4, 5, 6};

int clearedLines = 0;
int lastTetrimino = 0;
int event = 0;
int bag[TETROMINO_AMOUNT] = {0, 1, 2, 3, 4, 5, 6};
int grid[ROWS][COLS];
int moveDown = 0;
int tetrominoIndex = 0;

double speed = 20;
double speedStep = 0.5;

bool isGameOver = false;

Tetromino *currentTetrimino;
Tetromino tetriminos[TETROMINO_AMOUNT];

bool isInside(int row, int col)
{
  return row >= 0 && row < ROWS && col >= 0 && col < COLS;
}

bool isEmpty(int row, int col)
{
  if (isInside(row, col) && grid[row][col] == 0)
  {
    return true;
  }
  return false;
}

bool isRowFull(int row)
{
  for (int col = 0; col < COLS; col++)
  {
    if (grid[row][col] == EMPTY)
    {
      return false;
    }
  }
  return true;
}

void clearRow(int row)
{
  for (int col = 0; col < COLS; col++)
  {
    grid[row][col] = EMPTY;
  }
}

bool canMoveTo(int targetX, int targetY)
{
  for (int row = 0; row < TETROMINO_SIZE; row++)
  {
    for (int col = 0; col < TETROMINO_SIZE; col++)
    {
      if (currentTetrimino->positions[currentTetrimino->rotation_state][row][col] == SPOT)
      {
        if (!isEmpty(currentTetrimino->y + row + targetY, currentTetrimino->x + col + targetX))
        {
          return false;
        }
      }
    }
  }
  return true;
}

void generateBag()
{
  memcpy(bag, INITIAL_BAG, sizeof(bag));
  for (int row = 0; row < TETROMINO_AMOUNT; row++)
  {
    int randomIndex = rand() % TETROMINO_AMOUNT;
    int temp = bag[row];
    bag[row] = bag[randomIndex];
    bag[randomIndex] = temp;
  }
}

bool canRotate()
{
  for (int row = 0; row < TETROMINO_SIZE; row++)
  {
    for (int col = 0; col < TETROMINO_SIZE; col++)
    {
      if (currentTetrimino->positions[(currentTetrimino->rotation_state + 1) % TETROMINO_ROTATIONS][row][col] == SPOT)
      {
        if (!isEmpty(currentTetrimino->y + row, currentTetrimino->x + col))
        {
          return false;
        }
      }
    }
  }
  return true;
}

void drawGrid()
{
  for (int row = 0; row < ROWS; row++)
  {
    for (int col = 0; col < COLS; col++)
    {
      if (grid[row][col] == EMPTY)
      {
        gfx_filledRect(col * BLOCK_SIZE + OFFSET, row * BLOCK_SIZE + OFFSET, 
        (col + 1) * BLOCK_SIZE - 1 - OFFSET, 
        (row + 1) * BLOCK_SIZE - 1 - OFFSET, WHITE);
      }
      else
      {
        gfx_filledRect(col * BLOCK_SIZE + OFFSET, row * BLOCK_SIZE + OFFSET, 
        (col + 1) * BLOCK_SIZE - 1 - OFFSET, 
        (row + 1) * BLOCK_SIZE - 1 - OFFSET, grid[row][col]);
      }
    }
  }
}

void drawTetrimino(Tetromino tetrimino, int x, int y)
{
  for (int row = 0; row < TETROMINO_SIZE; row++)
  {
    for (int col = 0; col < TETROMINO_SIZE; col++)
    {
      if (tetrimino.positions[tetrimino.rotation_state][row][col] == SPOT)
      {
        gfx_filledRect((col + x + tetrimino.x) * BLOCK_SIZE + OFFSET, 
          (row + y + tetrimino.y) * BLOCK_SIZE + OFFSET, (col + x + 1 + tetrimino.x) * 
          BLOCK_SIZE - 1 - OFFSET, 
          (row + y + 1 + tetrimino.y) * BLOCK_SIZE - 1 - OFFSET, currentTetrimino->color);
      }
      if (tetrimino.positions[tetrimino.rotation_state][row][col] == PIVOT)
      {
        gfx_filledRect((col + x + tetrimino.x) * BLOCK_SIZE + OFFSET, 
          (row + y + tetrimino.y) * BLOCK_SIZE + OFFSET, 
          (col + x + 1 + tetrimino.x) * BLOCK_SIZE - 1 - OFFSET, 
          (row + y + 1 + tetrimino.y) * BLOCK_SIZE - 1 - OFFSET, CYAN);
      }
    }
  }
}

void updateScreen()
{
  gfx_updateScreen();
  SDL_Delay(REFRESH_RATE);
  gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1, BLACK);
}

void canSpawnNext()
{
  for (int row = 0; row < TETROMINO_SIZE; row++)
  {
    for (int col = 0; col < COLS; col++)
    {
      if (grid[row][col] != 0)
      {
        isGameOver = true;
        return;
      }
    }
  }
}

void assignNextTetrimino()
{
  canSpawnNext();

  bool allZero = true;

  for(int row = 0; row < 7; row++)
  {
    if(bag[row] != USED_PIECE)
    {
      tetrominoIndex = bag[row];
      bag[row] = USED_PIECE;
      allZero = false;
      break;
    }
  }

  if(allZero)
  {
    generateBag();
    tetrominoIndex = bag[0];
    bag[0] = USED_PIECE;
  }

  currentTetrimino = &tetriminos[tetrominoIndex];
  currentTetrimino->x = COLS / 2 - (TETROMINO_SIZE / 2);
  currentTetrimino->y = 0;
}

void moveBlockDown()
{
  if (moveDown >= speed && canMoveTo(0, DOWN))
  {
    currentTetrimino->y++;
    moveDown = 0;
  }
  else if (!canMoveTo(0, DOWN))
  {
    for (int row = 0; row < TETROMINO_SIZE; row++)
    {
      for (int col = 0; col < TETROMINO_SIZE; col++)
      {
        if (currentTetrimino->positions[currentTetrimino->rotation_state][row][col] != EMPTY)
        {
          grid[currentTetrimino->y + row][currentTetrimino->x + col] = currentTetrimino->color;
        }
      }
    }
    assignNextTetrimino();
  }
  moveDown++;
}

void increaseSpeed()
{
  clearedLines++;
  if (clearedLines % LEVEL_UP_CONDITION == 0 && speed >= MAX_SPEED)
  {
    speed -= speedStep;
  }
}

void checkRows()
{
  for (int row = 0; row < ROWS; row++)
  {
    if (isRowFull(row))
    {
      clearRow(row);
      increaseSpeed();
      for (int col = row; col > 0; col--)
      {
        for (int k = 0; k < COLS; k++)
        {
          grid[col][k] = grid[col - 1][k];
        }
      }
    }
  }
}

void rotatePiece()
{
  int x, y;
  int newX, newY;

  int tempX = currentTetrimino->x;
  int tempY = currentTetrimino->y;

  for(int row = 0; row < TETROMINO_SIZE; row++)
  {
    for(int col = 0; col < TETROMINO_SIZE; col++)
    {
      if(currentTetrimino->positions[currentTetrimino->rotation_state][row][col] == PIVOT)
      {
        x = col;
        y = row;
      }
    }
  }

  for(int row = 0; row < TETROMINO_SIZE; row++)
  {
    for(int col = 0; col < TETROMINO_SIZE; col++)
    {
      if(currentTetrimino->positions[(currentTetrimino->rotation_state + 1) % TETROMINO_ROTATIONS][row][col] == PIVOT)
      {
        newX = col;
        newY = row;
      }
    }
  }

  currentTetrimino->x = currentTetrimino->x - newX + x;
  currentTetrimino->y = currentTetrimino->y - newY + y;

  if(canRotate() == false)
  {
    currentTetrimino->x = tempX;
    currentTetrimino->y = tempY;
    return;
  }
  currentTetrimino->rotation_state = (currentTetrimino->rotation_state + 1) % TETROMINO_ROTATIONS;
}

void handleMovement(int event)
{
  switch (event)
  {
  case SDLK_LEFT:
    if (canMoveTo(LEFT, 0) == true)
    {
      currentTetrimino->x--;
    }
    break;
  case SDLK_RIGHT:
    if (canMoveTo(RIGHT, 0) == true)
    {
      currentTetrimino->x++;
    }
    break;
  case SDLK_SPACE:
    if (canRotate() == true)
    {
      rotatePiece();
    }
    break;
  case SDLK_ESCAPE:
    exit(0);
    break;
  case SDLK_RETURN:
    if (isGameOver)
    {
      memset(grid, 0, sizeof(int) * ROWS * COLS);
      isGameOver = false;
    }
    break;
  case SDLK_DOWN:
    while (canMoveTo(0, DOWN))
    {
      currentTetrimino->y++;
    }
    break;
  default:
    break;
  }
}

int main(int argc, char *argv[])
{
  srand(time(0));

  tetriminos[0] = lblock;
  tetriminos[1] = iblock;
  tetriminos[2] = jblock;
  tetriminos[3] = oblock;
  tetriminos[4] = sblock;
  tetriminos[5] = tblock;
  tetriminos[6] = zblock;

  SCREEN_WIDTH = COLS * BLOCK_SIZE;
  SCREEN_HEIGHT = ROWS * BLOCK_SIZE;

  generateBag();
  assignNextTetrimino();

  if (gfx_init())
  {
    exit(3);
  }

  while (1)
  {
    event = gfx_pollkey();
    handleMovement(event);

    updateScreen();
    drawGrid();

    if (isGameOver == false)
    {
      drawTetrimino(*currentTetrimino, 0, 0);
      moveBlockDown();
      checkRows();
    }
    else
    {
      char gameOverText[50] = "GAME OVER. CLEARED LINES: %d";
      sprintf(gameOverText, "GAME OVER. CLEARED LINES: %d", clearedLines);
      gfx_textout(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2, gameOverText, BLACK);
    }
  }

  gfx_getkey();
  return 0;
}