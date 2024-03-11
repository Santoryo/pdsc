#include "primlib.h"
#include <stdlib.h>
#include <unistd.h>
#include "math.h"

#define PI 3.14159265358979323846
#define REFRESH_RATE 16.666666


#define VERTICES 6
#define ALPHA 360.0 / VERTICES

int radius = 100;
int centerX, centerY;
int beta = 0;
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
	for(int i = 0; i <= VERTICES; i++)
		{
			int x1, x2, y1, y2;
			x1 = centerX + radius * SDL_cos(toRadians(i * ALPHA) + beta);
			y1 = centerY + radius * SDL_sin(toRadians(i * ALPHA) + beta);
			x2 = centerX + radius * SDL_cos(toRadians((i + 1) * ALPHA) + beta);
			y2 = centerY + radius * SDL_sin(toRadians(((i + 1) * ALPHA)) + beta);

			gfx_line(x1, y1, x2, y2, RED);
		}

}

void updateRotation()
{
	beta++;
	beta = beta % 360;
}

void updateSize()
{
	if(radius >= 500)
	{
		direction = -1;
	}
	else if(radius <= 100)
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
		drawPolygon();
		updateRotation();
		updateSize();
	}


	gfx_getkey();
	return 0;
}