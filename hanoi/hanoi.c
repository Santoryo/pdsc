#include "primlib.h"
#include <stdlib.h>
#include <unistd.h>
#include "math.h"

#define REFRESH_RATE 16.666666
#define PADDING 50

#define DISKS 9
#define STAKES 3
#define DIFFICULTY 5
#define RING_WIDTH 20
#define RING_HEIGHT 20


int centerX, centerY;
int event;
int containerWidth;

int keyBuffer[2] = {0, 0};

int stakes[STAKES][DISKS] = {	{5, 4, 3, 2, 1}, {5, 4, 3, 2, 1}, {0}	};

struct Rectangle
{
	int x1;
	int x2;
	int y1;
	int y2;
	int spacing;
};

struct Rectangle background;

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
    int elementWidth = 20;
    int totalWidth = background.x2 - background.x1;
    int totalSpacing = totalWidth - STAKES * elementWidth; 
    int spaceBetween = totalSpacing / (2 * STAKES + 1);

    for (int i = 0; i < STAKES; i++)
    {
        int x1 = background.x1 + spaceBetween * (2 * i + 1) + elementWidth * i;
        int x2 = x1 + elementWidth;
        gfx_filledRect(x1, background.y2, x2, centerY, GREEN);
    }
}

void drawStakes()
{
		int elementWidth = 200;
    int totalWidth = background.x2 - background.x1;
    int totalSpacing = totalWidth - STAKES * elementWidth; 
    int spaceBetween = totalSpacing / (2 * STAKES + 1);

    for(int i = 0; i < STAKES; i++)
    {
        int stakeX = background.x1 + spaceBetween * (2 * i + 1) + elementWidth * i;

        for(int j = 0; j < DISKS; j++)
        {
            if(stakes[i][j] != 0) {
                int diskWidth = stakes[i][j] * elementWidth / DISKS; // The width of the disk is proportional to its size
                int diskX = stakeX + (elementWidth - diskWidth) / 2; // The disk is centered on the stake
                int diskY = background.y2 - (j + 1) * RING_HEIGHT;
                gfx_filledRect(diskX, diskY, diskX + diskWidth, diskY + RING_HEIGHT, BLUE);
            }
        }
    }
}


void moveDisk(int fromStake, int toStake) {
    // Find the top disk on the source stake
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
                // The disk on the destination stake is smaller, so the move is not allowed
                return;
            }
            break;
        }
    }

    // The move is allowed, so remove the disk from the source stake
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
	// if(!(key <= 49 && key >= 49 + STAKES)) return;

	int target = key-48;

	if(keyBuffer[0] != 0 && keyBuffer[1] != 0)
	{
		moveDisk(keyBuffer[0], keyBuffer[1]);
	}
	else if(keyBuffer[0] == 0) {
		keyBuffer[0] == target;
	} 
	else if(keyBuffer[1] == 0) {
		keyBuffer[1] == target;
	}

	printf("KEY BUFFER: [%i %i] \n", keyBuffer[0], keyBuffer[1]);

	


}

int main(int argc, char *argv[])
{
	if (gfx_init())
	{
		exit(3);
	}

	while (1)
	{
		event = gfx_pollkey();
		if (event != -1)
		{
			printf("%i", event);
			handleKeys(event);

		}


		updateScreen();
		setupVariables();
		draw🛕();
		drawStakes();

		// rest of code
	}

	gfx_getkey();
	return 0;
}