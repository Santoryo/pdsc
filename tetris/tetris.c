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
const int INITIAL_BAG[7] = {0, 1, 2, 3, 4, 5, 6};

int clearedLines = 0;
int lastTetrimino = 0;
int event = 0;

int bag[7] = {0, 1, 2, 3, 4, 5, 6};
int grid[ROWS][COLS];
int moveDown = 0;
int tetrominoIndex = 0;
double speed = 12;

bool isGameOver = false;

Tetromino *currentTetrimino;
Tetromino tetriminos[7];

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

bool isRowFull(int row, int col)
{
  for (int i = 0; i < COLS; i++)
  {
    if (grid[row][i] == 0)
    {
      return false;
    }
  }
  return true;
}

void clearRow(int row)
{
  for (int i = 0; i < COLS; i++)
  {
    grid[row][i] = 0;
  }
}

bool canMoveDown()
{
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      if (currentTetrimino->positions[currentTetrimino->rotation_state][i][j] == 1)
      {
        if (!isEmpty(currentTetrimino->y + i + 1, currentTetrimino->x + j))
        {
          return false;
        }
      }
    }
  }
  return true;
}

bool canMoveLeft()
{
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      if (currentTetrimino->positions[currentTetrimino->rotation_state][i][j] == 1)
      {
        if (!isEmpty(currentTetrimino->y + i, currentTetrimino->x + j - 1))
        {
          return false;
        }
      }
    }
  }
  return true;
}

bool canMoveRight()
{
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      if (currentTetrimino->positions[currentTetrimino->rotation_state][i][j] == 1)
      {
        if (!isEmpty(currentTetrimino->y + i, currentTetrimino->x + j + 1))
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
  for (int i = 0; i < 7; i++)
  {
    int randomIndex = rand() % 7;
    int temp = bag[i];
    bag[i] = bag[randomIndex];
    bag[randomIndex] = temp;
  }
}

bool canRotate()
{
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      if (currentTetrimino->positions[(currentTetrimino->rotation_state + 1) % 4][i][j] == 1)
      {
        if (!isEmpty(currentTetrimino->y + i, currentTetrimino->x + j))
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
  for (int i = 0; i < ROWS; i++)
  {
    for (int j = 0; j < COLS; j++)
    {
      if (grid[i][j] == 0)
      {
        gfx_filledRect(j * BLOCK_SIZE + OFFSET, i * BLOCK_SIZE + OFFSET, (j + 1) * BLOCK_SIZE - 1 - OFFSET, (i + 1) * BLOCK_SIZE - 1 - OFFSET, WHITE);
      }
      else
      {
        gfx_filledRect(j * BLOCK_SIZE + OFFSET, i * BLOCK_SIZE + OFFSET, (j + 1) * BLOCK_SIZE - 1 - OFFSET, (i + 1) * BLOCK_SIZE - 1 - OFFSET, grid[i][j]);
      }
    }
  }
}

void drawTetrimino(Tetromino tetrimino, int x, int y)
{
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      if (tetrimino.positions[tetrimino.rotation_state][i][j] == 1)
      {
        gfx_filledRect((j + x + tetrimino.x) * BLOCK_SIZE + OFFSET, (i + y + tetrimino.y) * BLOCK_SIZE + OFFSET, (j + x + 1 + tetrimino.x) * BLOCK_SIZE - 1 - OFFSET, (i + y + 1 + tetrimino.y) * BLOCK_SIZE - 1 - OFFSET, currentTetrimino->color);
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
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < COLS; j++)
    {
      if (grid[i][j] != 0)
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

  printBag();

  bool allZero = true;

  for(int i = 0; i < 7; i++)
  {
    if(bag[i] != -1)
    {
      tetrominoIndex = bag[i];
      bag[i] = -1;
      allZero = false;
      break;
    }
  }

  if(allZero)
  {
    generateBag();
    tetrominoIndex = bag[0];
    bag[0] = -1;
  }

  currentTetrimino = &tetriminos[tetrominoIndex];
  currentTetrimino->x = COLS / 2 - 2;
  currentTetrimino->y = 0;
}

void moveBlockDown()
{
  if (moveDown >= speed && canMoveDown())
  {
    currentTetrimino->y++;
    moveDown = 0;
  }
  else if (!canMoveDown())
  {
    for (int i = 0; i < 4; i++)
    {
      for (int j = 0; j < 4; j++)
      {
        if (currentTetrimino->positions[currentTetrimino->rotation_state][i][j] == 1)
        {
          grid[currentTetrimino->y + i][currentTetrimino->x + j] = currentTetrimino->color;
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
  if (clearedLines % 3 == 0)
  {
    speed -= 0.5;
  }
}

void checkRows()
{
  for (int i = 0; i < ROWS; i++)
  {
    if (isRowFull(i, 0))
    {
      clearRow(i);
      increaseSpeed();
      for (int j = i; j > 0; j--)
      {
        for (int k = 0; k < COLS; k++)
        {
          grid[j][k] = grid[j - 1][k];
        }
      }
    }
  }
}

void handleMovement(int event)
{
  switch (event)
  {
  case SDLK_LEFT:
    if (canMoveLeft() == true)
    {
      currentTetrimino->x--;
    }
    break;
  case SDLK_RIGHT:
    if (canMoveRight() == true)
    {
      currentTetrimino->x++;
    }
    break;
  case SDLK_UP:
    if (canRotate() == true)
    {
      currentTetrimino->rotation_state = (currentTetrimino->rotation_state + 1) % 4;
    }
    break;
  case SDLK_DOWN:
    if (canMoveDown() == true)
    {
      currentTetrimino->y++;
    }
    break;
  case SDLK_ESCAPE:
    exit(0);
    break;
  case SDLK_SPACE:
    if (isGameOver)
    {
      memset(grid, 0, sizeof(grid[0][0]) * ROWS * COLS);
      isGameOver = false;
    }
    else
    {
      while (canMoveDown())
      {
        currentTetrimino->y++;
      }
    }
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
      char gameOverText[50];
      sprintf(gameOverText, "GAME OVER. CLEARED LINES: %d", clearedLines);
      printf("%s\n", gameOverText);
      gfx_textout(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2, gameOverText, RED);
    }
  }

  gfx_getkey();
  return 0;
}