#include "primlib.h"
#include <stdlib.h>

// GAME SETTINGS
#define TOWERS 8
#define RINGS 4

// SDL CONSTANTS
#define REFRESH_RATE 16.666666
#define ANIMATION_STEP 10

// TOWERS CONSTANTS
#define STAKE_WIDTH 20
#define STAKE_HEIGHT 300

// RINGS CONSTANTS
#define RING_HEIGHT STAKE_HEIGHT / RINGS
#define RING_BASE_WIDTH 20
#define RING_MAX_WIDTH gfx_screenWidth() / (4 * TOWERS)

#if RINGS > 100
#error "RINGS can be up to 100"
#endif

// ANIMATION STATE-Y
#define Y_MAX 10

int event = -1;

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
  for (int idx = 0; idx < TOWERS; idx++)
  {
    stakes[idx].topStake = 0;
    stakes[idx].x1 = idx * gfx_screenWidth() / TOWERS + gfx_screenWidth() / (TOWERS * 2) - STAKE_WIDTH / 2;
    stakes[idx].x2 = idx * gfx_screenWidth() / TOWERS + gfx_screenWidth() / (TOWERS * 2) + STAKE_WIDTH / 2;
    stakes[idx].y1 = gfx_screenHeight() - STAKE_HEIGHT;
    stakes[idx].y2 = gfx_screenHeight();

    if (idx == 0)
    {
      stakes[idx].topStake = RINGS;
      for (int j = 0; j < RINGS; j++)
      {
        stakes[idx].rings[j] = RINGS - j;
      }
    }
  }
}

void checkWinCondition()
{
  for (int idx = 1; idx < TOWERS; idx++)
  {
    if (stakes[idx].topStake == RINGS)
    {
      gameCompleted = 1;
    }
  }
}

void displayWinMessage()
{
  gfx_textout(gfx_screenWidth() / 2, Y_MAX, "Press ESC to exit or ENTER to restart", WHITE);
}

void drawStakes()
{
  for (int tower = 0; tower < TOWERS; tower++)
  {
    gfx_filledRect(stakes[tower].x1, stakes[tower].y1, stakes[tower].x2, stakes[tower].y2, WHITE);
  }
}

void drawRings()
{
  for (int tower = 0; tower < TOWERS; tower++)
  {
    for (int stake = 0; stake < stakes[tower].topStake; stake++)
    {
      int ringWidth = RING_BASE_WIDTH + stakes[tower].rings[stake] * (RING_MAX_WIDTH / RING_BASE_WIDTH);
      int ringX = tower * gfx_screenWidth() / TOWERS + gfx_screenWidth() / (TOWERS * 2) - ringWidth / 2;
      int ringY = gfx_screenHeight() - (stake + 1) * RING_HEIGHT;

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
  int ringWidth = RING_BASE_WIDTH + stakes[sourceStake].rings[ringIndex] * (RING_MAX_WIDTH / RING_BASE_WIDTH);
  int ringX = sourceStake * gfx_screenWidth() / TOWERS + gfx_screenWidth() / (TOWERS * 2) - ringWidth / 2;
  int ringY = gfx_screenHeight() - (ringIndex + 1) * RING_HEIGHT;

  int direction = 1;
  if (targetStake < sourceStake)
    direction = -1;

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

int returnKey(int event)
{
  switch (event)
  {
  case SDLK_1:
    return 1;
    break;
  case SDLK_2:
    return 2;
  case SDLK_3:
    return 3;
  case SDLK_4:
    return 4;
  case SDLK_5:
    return 5;
  case SDLK_6:
    return 6;
  case SDLK_7:
    return 7;
  case SDLK_8:
    return 8;
  case SDLK_9:
    return 9;
  case SDLK_0:
    return 10;
  default:
    return -1;
  }
}

int main(int argc, char *argv[])
{
  int sourceStake = -1;

  if (gfx_init())
    exit(3);

  setupStakes();

  while (1)
  {
    event = gfx_pollkey();

    if (gameCompleted && event == SDLK_RETURN)
    {
      setupStakes();
      gameCompleted = 0;
    }

    if(event == SDLK_r)
    {
      sourceStake = -1;
      event = -1;
    }

    if (event == SDLK_ESCAPE) exit(0);


    event = returnKey(event);
    if(event < 0 || event > TOWERS) event = -1;

    if (event != -1)
    {
      int stake = event - 1;
      int topStake = stakes[stake].topStake;

      if (sourceStake == -1 && topStake > 0)
      {
        sourceStake = stake;
      }
      else if (sourceStake != -1 && topStake < RINGS && (topStake == 0 || stakes[stake].rings[topStake - 1] > stakes[sourceStake].rings[topStake - 1]))
      {
        stakes[stake].rings[topStake] = stakes[sourceStake].rings[stakes[sourceStake].topStake - 1];
        stakes[sourceStake].topStake--;
        animateMoveRing(sourceStake, stake);
        stakes[stake].topStake++;
        sourceStake = -1;
      }
    }

    if (gameCompleted) displayWinMessage();

    updateScreen();
    checkWinCondition();
  }

  gfx_getkey();
  return 0;
}