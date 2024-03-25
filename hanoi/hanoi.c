#include "primlib.h"
#include <stdlib.h>
#include <unistd.h>
#include "math.h"

#define REFRESH_RATE 16.666666
#define PADDING 50

#define DISKS 12
#define STAKES 3
#define DIFFICULTY 5
#define RING_WIDTH 200
#define RING_HEIGHT 20

#define STAKE_WIDTH 20


int centerX, centerY;
int event;
int containerWidth;

int state = 0;
int keyBuffer[2] = {-1, -1};
int stakes[STAKES][DISKS];

float y = 0;

struct Rectangle
{
	int x1;
	int x2;
	int y1;
	int y2;
	int spacing;
};

struct Rectangle background;

void setupStakes()
{
	for(int stake = 0; stake < DISKS; stake++)
	{
		printf("%i", stake);
		stakes[0][stake] = DISKS - stake;
	}
}

void updateScreen()
{
	gfx_updateScreen();
	SDL_Delay(REFRESH_RATE);
	gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1, BLACK);
}

void setupVariables()
{
	centerX = gfx_screenWidth() / 2;
	centerY = gfx_screenHeight() / 2;

	background.x1 = PADDING;
	background.x2 = gfx_screenWidth() - PADDING;
	background.y1 = PADDING;
	background.y2 = gfx_screenHeight() - PADDING;
	background.spacing = (background.x2 - background.x1) / STAKES;
}

void draw🛕()
{
    for (int i = 0; i < STAKES; i++)
    {
        int x1 = background.x1 + (i * background.spacing) + STAKE_WIDTH * i + 90;
        int x2 = x1 + STAKE_WIDTH;
        gfx_filledRect(x1, background.y2, x2, centerY, GREEN);
    }
}

void drawStakes()
{

    for(int i = 0; i < STAKES; i++)
    {
        int stakeX = background.x1 + (i * background.spacing) + STAKE_WIDTH * i;

        for(int j = 0; j < DISKS; j++)
        {
            if(stakes[i][j] != 0) {
                int diskWidth = stakes[i][j] * RING_WIDTH / DISKS; 
                int diskX = stakeX + RING_WIDTH / 2 - diskWidth / 2;
                int diskY = background.y2 - (j + 1) * RING_HEIGHT;
                gfx_filledRect(diskX, diskY, diskX + diskWidth, diskY + RING_HEIGHT, BLUE);
            }
        }
    }
}


void moveDisk(int fromStake, int toStake) {
    int topDisk = 0;
    int topDiskIndex = 0;
    for(int j = DISKS - 1; j >= 0; j--) {
        if(stakes[fromStake][j] != 0) {
            topDisk = stakes[fromStake][j];
            topDiskIndex = j;
            break;
        }
    }

    // Check the size of the top disk on the destination stake
    for(int j = DISKS - 1; j >= 0; j--) {
        if(stakes[toStake][j] != 0) {
            if(stakes[toStake][j] < topDisk) {
                return;
            }
            break;
        }
    }

    // The move is allowed, so remove the disk from the source stake
		moveUp(topDiskIndex, fromStake, toStake);
    stakes[fromStake][topDiskIndex] = 0;

    // Place the disk on top of the destination stake
    for(int j = 0; j < DISKS; j++) {
        if(stakes[toStake][j] == 0) {
            stakes[toStake][j] = topDisk;
            break;
        }
    }
}

void handleKeys(char key)
{
	int target = key - 49;
	if(target < 0 && target > STAKES) return;


	if(keyBuffer[0] == -1) {
		keyBuffer[0] = target;
	} 
	else if(keyBuffer[1] == -1) {
		keyBuffer[1] = target;
		moveDisk(keyBuffer[0], keyBuffer[1]);
		keyBuffer[0] = -1;
		keyBuffer[1] = -1;
	}

}

void moveUp(int j, int i, int k)
{
	float y = background.y1 + (j * RING_HEIGHT);
	int stakeX = background.x1 + (i * background.spacing) + STAKE_WIDTH * i;
  int diskWidth = stakes[i][j] * RING_WIDTH / DISKS; 
  int diskX = stakeX + RING_WIDTH / 2 - diskWidth / 2;
	int diskY = background.y2 - (j + 1) * RING_HEIGHT;

	int stakeEndpoint = background.x1 + (k * background.spacing) + STAKE_WIDTH * k;
	int endPointX = stakeEndpoint + RING_WIDTH / 2 - diskWidth / 2;
	float x = diskX;

	int state = 0;

	while(state == 0)
	{
			gfx_filledRect(diskX, diskY + y, diskX + diskWidth, diskY + RING_HEIGHT + y, RED);
			y -= 4.00;
			updateScreen();
			draw🛕();
			drawStakes();
			if(diskY + y <= PADDING) { state = 1; }
	}

	state = 0;

	while(state == 0)
	{
			gfx_filledRect(diskX + x, diskY + y, diskX + x + diskWidth, diskY + RING_HEIGHT + y, RED);
			x += 4.00;
			updateScreen();
			draw🛕();
			drawStakes();
			if(diskX + x >= endPointX) { state = 1; }

	}  

	state = 0;

	while(state == 0)
	{
			gfx_filledRect(diskX + x, diskY + y, diskX + diskWidth + x, diskY + RING_HEIGHT + y, RED);
			y += 4.00;
			printf("xdd\n%f", diskY + y);
			updateScreen();
			draw🛕();
			drawStakes();
			if(diskY + y >= gfx_screenHeight() - PADDING - (RING_HEIGHT * j)) { state = 1; }
	}

}

void moveTo(int j, int i, int k)
{
	float y = PADDING;
	float x = PADDING;
	int stakeX = background.x1 + (i * background.spacing) + STAKE_WIDTH * i;
	int endPoint = background.x1 + (k * background.spacing) + STAKE_WIDTH * k;
	printf("%i", endPoint);
  int diskWidth = stakes[i][j] * RING_WIDTH / DISKS; 
  int diskX = stakeX + RING_WIDTH / 2 - diskWidth / 2;
	int diskY = background.y2 - (j + 1) * RING_HEIGHT;

	int state = 0;
	while(state == 0)
		{
			gfx_filledRect(diskX + x, diskY + y, diskX + diskWidth + x, diskY + RING_HEIGHT + y, RED);
			y += 2.0;
			updateScreen();
			draw🛕();
			drawStakes();
			if(diskX <= endPoint) { state = 1; }
		} 
}

int main(int argc, char *argv[])
{
	if (gfx_init())
	{
		exit(3);
	}

	setupStakes();
	setupVariables();

	while (1)
	{
		event = gfx_pollkey();
		if (event != -1)
		{
			printf("%i", event);
			handleKeys(event);
		}


		updateScreen();
		draw🛕();
		drawStakes();

		// rest of code
	}

	gfx_getkey();
	return 0;
}