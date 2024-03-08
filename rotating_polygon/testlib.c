#include "primlib.h"
#include <stdlib.h>
#include <unistd.h>
#include "math.h"

#define PI 3.14159265358979323846
#define REFRESH_RATE 16.666


int const VERTICES = 9;
int RADIUS = 100;
int alpha = 360.0 / VERTICES;
int center, centerY;
int beta = 0;


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

void calculatePolygon()
{

}

int main(int argc, char* argv[])
{
	if (gfx_init()) {
		exit(3);
	}

	center = gfx_screenWidth() / 2;
	centerY = gfx_screenHeight() / 2;

	while(1)
	{

					   int x1, y1, x2, y2;

		for(int i = 0; i <= VERTICES; i++)
		{
			x1 = center + RADIUS * SDL_cos(toRadians(i * alpha) + beta);
			y1 = centerY + RADIUS * SDL_sin(toRadians(i * alpha) + beta);
			x2 = center + RADIUS * SDL_cos(toRadians((i + 1) * alpha) + beta);
			y2 = centerY + RADIUS * SDL_sin(toRadians(((i + 1) * alpha)) + beta);

			gfx_line(x1, y1, x2, y2, RED);
		}

		updateScreen();
		beta++;
		RADIUS++;
	}


	gfx_getkey();
	return 0;
}