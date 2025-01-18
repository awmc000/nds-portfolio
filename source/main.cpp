/*---------------------------------------------------------------------------------

	Basic template code for starting a DS app

---------------------------------------------------------------------------------*/
#include <nds.h>
#include <gl2d.h>
#include <cstdio>
#include <cmath>
#include "maze.hpp"
#include "vector.hpp"
#include "raycaster.hpp"

// Relminator's constants
#define HALF_WIDTH (SCREEN_WIDTH/2)
#define HALF_HEIGHT (SCREEN_HEIGHT/2)
#define BRAD_PI (1 << 14)

/**
 * NOT BY ME! By Relminator
 * Radially displaced pixels demo
*/
void pixels(int frame);


/**
 * Untextured raycaster: Draw a frame
*/
void raycast(
	Maze * maze,		// The map
	struct v2f * pos,	// Player fine position
	struct v2f * dir,	// Player looking direction
	struct v2f * plane	// Camera plane
);

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------

	videoSetMode(MODE_5_3D);
	glScreen2D();

	consoleDemoInit();
	iprintf("Hello World!");

	Maze * maze = new Maze();
	maze->build();
	struct v2f pos = {2, 2};
	struct v2f dir = {1, 0};
	struct v2f plane = {0.0, 0.66};
	Raycaster * rc = new Raycaster(maze, &pos, &dir, &plane);

	int frame = 0;
	int threshold = 100;

	while(1) {
		frame++;

		rc->drawFrame();
		// raycast(maze, &pos, &dir, &plane);
		glFlush(0);

		swiWaitForVBlank();
		scanKeys();
		int pressed = keysDown();
		
		if (pressed & KEY_START) break;
	
		if (pressed & KEY_UP) pos.y--;
		if (pressed & KEY_DOWN) pos.y++;
		if (pressed & KEY_LEFT) pos.x--;
		if (pressed & KEY_RIGHT) pos.x++;

		if (pressed & KEY_L) dir.x -= 0.05;
		if (pressed & KEY_R) dir.x += 0.05;

		if (pressed & KEY_A) threshold += 10;
		if (pressed & KEY_B) threshold -= 10;

		consoleClear();
		iprintf("x: %d, y: %d, frame %d th %d", (int) pos.x,(int) pos.y, frame, threshold);
	}
}

void pixels(int frame) {
	// Elastic radius
	int radius = 40+ (abs(sinLerp(frame*20)*80) >> 12);
	
	// Do some funky color cycling
	int red = abs(sinLerp(frame*220)*31) >> 12 ;
	int green = abs(sinLerp(frame*140)*31) >> 12 ;
	int blue = abs(sinLerp(frame*40)*31) >> 12 ;
	

	// speed opf animation
	int i = ( frame* 140 ) & 32767;
	
	// duh!
	int angle;
	
	// set up GL2D for 2d mode
	glBegin2D();

		// Draw a full revolution of some radially dispalced pixels
		for( angle = 0; angle < BRAD_PI*2; angle += 64)
		{
		
			int a2 = angle + i;
			int x = cosLerp(angle*2) * radius;
			int y = sinLerp(x/32 + a2) * radius;
			x = cosLerp(y/64 + angle) * (radius+20);
			y = sinLerp(x/64 + a2) * radius;
			int x2 = -y;
			int y2 = x;
			
			glPutPixel( HALF_WIDTH + (x >> 12), 
						HALF_HEIGHT + (y >> 12), 
						RGB15(red,green,blue)
					  );
			glPutPixel( HALF_WIDTH + (x2 >> 12), 
						HALF_HEIGHT + (y2 >> 12), 
						RGB15(green,blue,red)
					  );
			
		}
		
	glEnd2D();
}

void raycast(Maze * maze, struct v2f * pos, struct v2f * dir, struct v2f * plane) {
	glBegin2D();
	// For each vertical stripe...
	for (int x = 0; x < SCREEN_WIDTH; x++) {
		double cameraX = 2 * x / (double)SCREEN_WIDTH - 1;
		
		struct v2f rayDir = {
			dir->x + (plane->x * cameraX),
			dir->y + (plane->y * cameraX)
		};

		// Tile position
		struct v2i mapPos = {
			(int) pos->x,
			(int) pos->y
		};

		struct v2f sideDist = {
			0.0,
			0.0
		};

		// Length of ray from one x-line to the next x-line, and y-line to next y-line
		struct v2f deltaDistance = {
			std::fabs(1.0 / rayDir.x),
			std::fabs(1.0 / rayDir.y)
		};

		double perpWallDist;

		struct v2i step = {
			0,
			0
		};

		bool hit = false;
		int side;

		// Horizontal step direction
		if (rayDir.x < 0) {
			step.x = -1; 
			sideDist.x = (pos->x - mapPos.x) * deltaDistance.x;
		} else {
			step.x = 1;
			sideDist.x = (mapPos.x + 1.0 - pos->x) * deltaDistance.x;
		}

		// Vertical step direction
		if (rayDir.y < 0) {
			step.y = -1; 
			sideDist.y = (pos->y - mapPos.y) * deltaDistance.y;
		} else {
			step.y = 1;
			sideDist.y = (mapPos.y + 1.0 - pos->y) * deltaDistance.y;
		}

		// DDA algorithm
		while (!hit) {
			if (sideDist.x < sideDist.y) {
				sideDist.x += deltaDistance.x;
				mapPos.x += step.x;
				side = 0;
			} else {
				sideDist.y += deltaDistance.y;
				mapPos.y += step.y;
				side = 1;
			}
			if (maze->getCell(mapPos.y, mapPos.x) > 0) {
				hit = true;
			}
		}

		if (side == 0) {
			perpWallDist = (sideDist.x - deltaDistance.x);
		} else {
			perpWallDist = (sideDist.y - deltaDistance.y);
		}

		// Compute how to draw the line
		int lineHeight = (int) (SCREEN_HEIGHT / perpWallDist);

		int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
		if (drawStart < 0) drawStart = 0;
		
		int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
		if (drawEnd >= SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT - 1;

		int color;

		switch (maze->getCell(mapPos.y, mapPos.x)) {
			case 1:
				color = RGB15(31, 2, 2);
				break;
			default:
				color = RGB15(15, 0, 15);
				break;
		}
		
		if (side == 1) {
			color = RGB15(15, 2, 4);
		}
		
		glLine(x, drawStart, x, drawEnd, color);
	}
	glEnd2D();
}