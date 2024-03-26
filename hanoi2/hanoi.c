#include "primlib.h"
#include <stdlib.h>
#include <unistd.h>
#include "math.h"

// GAME SETTINGS
#define TOWERS 4
#define RINGS 2

// SDL CONSTANTS
#define REFRESH_RATE 16.666666
#define ANIMATION_STEP 10

// TOWERS CONSTANTS
#define STAKE_WIDTH 20
#define STAKE_HEIGHT 400

// RINGS CONSTANTS
#define RING_HEIGHT 20
#define RING_BASE_WIDTH 20

// ANIMATION STATE-Y
#define Y_MAX 100

typedef struct
{
  int rings[RINGS];
  int topStake;
  int x1;
  int x2;
  int y1;
  int y2;
} Stake;

Stake stakes[TOWERS];

int gameCompleted = 0;

void setupStakes()
{
  for (int i = 0; i < TOWERS; i++)
  {
    stakes[i].topStake = 0;
    stakes[i].x1 = i * gfx_screenWidth() / TOWERS + gfx_screenWidth() / (TOWERS * 2) - STAKE_WIDTH / 2;
    stakes[i].x2 = i * gfx_screenWidth() / TOWERS + gfx_screenWidth() / (TOWERS * 2) + STAKE_WIDTH / 2;
    stakes[i].y1 = gfx_screenHeight() - STAKE_HEIGHT;
    stakes[i].y2 = gfx_screenHeight();

    if (i == 0)
    {
      stakes[i].topStake = RINGS;
      for (int j = 0; j < RINGS; j++)
      {
        stakes[i].rings[j] = RINGS - j;
      }
    }
  }
}

void checkWinCondition()
{
  for (int i = 1; i < TOWERS; i++)
  {
    if (stakes[i].topStake == RINGS)
    {
      gameCompleted = 1;
    }
  }
}

void displayWinMessage()
{
  gfx_textout(gfx_screenWidth() / 2 - 50, Y_MAX, "Press ESC to exit or ENTER to restart", WHITE);
}

void drawStakes()
{
  for (int i = 0; i < TOWERS; i++)
  {
    gfx_filledRect(stakes[i].x1, stakes[i].y1, stakes[i].x2, stakes[i].y2, WHITE);
  }
}

void drawRings()
{
  for (int i = 0; i < TOWERS; i++)
  {
    for (int j = 0; j < stakes[i].topStake; j++)
    {
      int ringWidth = RING_HEIGHT + stakes[i].rings[j] * RING_BASE_WIDTH;
      int ringX = i * gfx_screenWidth() / TOWERS + gfx_screenWidth() / (TOWERS * 2) - ringWidth / 2;
      int ringY = gfx_screenHeight() - (j + 1) * RING_HEIGHT;

      gfx_filledRect(ringX, ringY, ringX + ringWidth, ringY + RING_HEIGHT, RED);
    }
  }
}

void updateScreen()
{
  gfx_updateScreen();
  SDL_Delay(REFRESH_RATE);
  gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1, BLACK);
  drawStakes();
  drawRings();
}

void animateMoveRing(int sourceStake, int targetStake)
{
  int ringIndex = stakes[sourceStake].topStake;
  int ringWidth = RING_BASE_WIDTH + stakes[sourceStake].rings[ringIndex] * RING_BASE_WIDTH;
  int ringX = sourceStake * gfx_screenWidth() / TOWERS + gfx_screenWidth() / (TOWERS * 2) - ringWidth / 2;
  int ringY = gfx_screenHeight() - (ringIndex + 1) * RING_HEIGHT;

  int direction = 1;
  if (targetStake < sourceStake) direction = -1;

  while (ringY >= Y_MAX)
  {
    gfx_filledRect(ringX, ringY, ringX + ringWidth, ringY + RING_HEIGHT, GREEN);
    ringY = ringY - ANIMATION_STEP;
    updateScreen();
  }
  while (ringY <= Y_MAX && ringX != targetStake * gfx_screenWidth() / TOWERS + gfx_screenWidth() / (TOWERS * 2) - ringWidth / 2)
  {
    gfx_filledRect(ringX, ringY, ringX + ringWidth, ringY + RING_HEIGHT, GREEN);
    ringX = ringX + direction * ANIMATION_STEP;
    updateScreen();
  }
  while (ringY <= gfx_screenHeight() - (stakes[targetStake].topStake + 1) * RING_HEIGHT)
  {
    gfx_filledRect(ringX, ringY, ringX + ringWidth, ringY + RING_HEIGHT, GREEN);
    ringY = ringY + ANIMATION_STEP;
    updateScreen();
  }
}

int main(int argc, char *argv[])
{
  int event = 0;
  if (gfx_init())
  {
    exit(3);
  }

  setupStakes();

  int sourceStake = -1;

  while (1)
  {
    event = gfx_pollkey();

    if (event >= '1' && event <= '0' + TOWERS)
    {
      int stake = event - '1';

      if (sourceStake == -1)
      {
        if (stakes[stake].topStake > 0) sourceStake = stake;
      }
      else
      {
        if (stakes[stake].topStake < RINGS && (stakes[stake].topStake == 0 || stakes[stake].rings[stakes[stake].topStake - 1] > stakes[sourceStake].rings[stakes[sourceStake].topStake - 1]))
        {
          stakes[stake].rings[stakes[stake].topStake] = stakes[sourceStake].rings[stakes[sourceStake].topStake - 1];
          stakes[sourceStake].topStake--;
          animateMoveRing(sourceStake, stake);
          stakes[stake].topStake++;
          sourceStake = -1;
        }
      }
    }

    if (event == SDLK_ESCAPE)
    {
      exit(0);
    }

    if (gameCompleted)
    {
      displayWinMessage();
    }

    if (gameCompleted && event == SDLK_RETURN)
    {
      setupStakes();
      gameCompleted = 0;
    }

    updateScreen();
    checkWinCondition();
  }

  gfx_getkey();
  return 0;
}