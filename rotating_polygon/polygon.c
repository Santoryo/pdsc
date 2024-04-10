#include "primlib.h"
#include <stdlib.h>
#include <unistd.h>
#include "math.h"

#define PI 3.14159265358979323846
#define REFRESH_RATE 16.666666
#define VERTICES 7
#define ALPHA 360.0 / VERTICES
#define MAX_RADIUS 500
#define MIN_RADIUS 100
#define STEP 9910.1

float beta = 0;

int radius = 100;
int centerX, centerY;
int event;
int direction = 1;


double toRadians(int degrees)
{
	return degrees * PI / 180;
}

void updateScreen()
{
	gfx_updateScreen();
	SDL_Delay(REFRESH_RATE);
	gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1, BLACK);

}

void drawPolygon()
{
	for(int idx = 0; idx <= VERTICES; idx++)
		{
			int x1, x2, y1, y2;
			x1 = centerX + radius * SDL_cos(toRadians(idx * ALPHA) + beta);
			y1 = centerY + radius * SDL_sin(toRadians(idx * ALPHA) + beta);
			x2 = centerX + radius * SDL_cos(toRadians((idx + 1) * ALPHA) + beta);
			y2 = centerY + radius * SDL_sin(toRadians(((idx + 1) * ALPHA)) + beta);

			gfx_line(x1, y1, x2, y2, GREEN);
		}

}

void updateRotation()
{
	beta = beta + STEP;
	
	while (beta >= 360.0)
	{
		beta -= 360.0;
	}
}

void updateSize()
{
	if(radius >= MAX_RADIUS)
	{
		direction = -1;
	}
	else if(radius <= MIN_RADIUS)
	{
		direction = 1;
	}

	radius = radius + direction;
}

int main(int argc, char* argv[])
{
	if (gfx_init()) {
		exit(3);
	}

	centerX = gfx_screenWidth() / 2;
	centerY = gfx_screenHeight() / 2;

	while(1)
	{
		event = gfx_pollkey();
		if(event != -1) exit(0);

		updateScreen();

		gfx_filledRect(gfx_screenWidth() - 100, gfx_screenHeight() - 100, gfx_screenWidth(), gfx_screenHeight(), RED);


		drawPolygon();
		updateRotation();
		updateSize();
	}


	gfx_getkey();
	return 0;
}