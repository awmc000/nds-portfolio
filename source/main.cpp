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

#define ROTATE_INCREMENT 0.25
/**
 * NOT BY ME! By Relminator
 * Radially displaced pixels demo
*/
void pixels(int frame);

/**
 * Reads and handles input, acting on player position, direction, and camera plane
*/
void handleInput(struct v2f * pos, struct v2f * dir, struct v2f * plane);

/**
 * Draws a minimap of sorts over top of the display
*/
void drawMinimap(Maze * maze);

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------

	videoSetMode(MODE_5_3D);

	videoSetModeSub(MODE_0_2D);
	vramSetBankC(VRAM_C_SUB_BG);
	PrintConsole * def = consoleGetDefault();
	consoleInit(NULL, def->bgLayer, BgType_Text4bpp, BgSize_T_256x256, def->mapBase, def->gfxBase, false, true);

	glScreen2D();

	// use VRAM_C for minimap, etc.

	Maze * maze = new Maze();
	maze->build();
	struct v2f pos = {2, 2};
	struct v2f dir = {1, 0};
	struct v2f plane = {0.0, 0.66};
	Raycaster * rc = new Raycaster(maze, &pos, &dir, &plane);

	int frame = 0;

	while(1) {
		frame++;

		pixels(frame);

		rc->drawFrame();
		drawMinimap(maze);
		glFlush(0);

		swiWaitForVBlank();
		
		handleInput(&pos, &dir, &plane);

		maze->visit((int)pos.y, (int)pos.x);

		consoleClear();
	}
}

void handleInput(struct v2f * pos, struct v2f * dir, struct v2f * plane) {
	scanKeys();
	int pressed = keysDownRepeat();
		
	if (pressed & KEY_START) 	exit(0);
	
	if (pressed & KEY_UP) {
		pos->x += dir->x;
		pos->y += dir->y;
	}

	if (pressed & KEY_DOWN) {
		pos->x -= dir->x;
		pos->y -= dir->y;
	}

	if (pressed & KEY_LEFT) 	pos->x -= dir->x;
	if (pressed & KEY_RIGHT) 	pos->x += dir->x;

	if (pressed & KEY_L) {
		double oldDirX = dir->x;
		dir->x = dir->x * cos(ROTATE_INCREMENT) - dir->y * sin(ROTATE_INCREMENT);
		dir->y = oldDirX * sin(ROTATE_INCREMENT) + dir->y * cos(ROTATE_INCREMENT);

		double oldPlaneX = plane->x;
		plane->x = plane->x * cos(ROTATE_INCREMENT) - plane->y * sin(ROTATE_INCREMENT);
		plane->y = oldPlaneX * sin(ROTATE_INCREMENT) + plane->y * cos(ROTATE_INCREMENT);
	}
	if (pressed & KEY_R) {
		double oldDirX = dir->x;
		dir->x = dir->x * cos(-ROTATE_INCREMENT) - dir->y * sin(-ROTATE_INCREMENT);
		dir->y = oldDirX * sin(-ROTATE_INCREMENT) + dir->y * cos(-ROTATE_INCREMENT);

		double oldPlaneX = plane->x;
		plane->x = plane->x * cos(-ROTATE_INCREMENT) - plane->y * sin(-ROTATE_INCREMENT);
		plane->y = oldPlaneX * sin(-ROTATE_INCREMENT) + plane->y * cos(-ROTATE_INCREMENT);
	}
}

void drawMinimap(Maze * maze) {
	glBegin2D();
	for (int y = 0; y < MAZE_HEIGHT; y++) {
		for (int x = 0; x < MAZE_WIDTH; x++) {
			if (maze->cellVisited(y, x)) {
				glPutPixel(y, x + (SCREEN_WIDTH / 2), RGB15(31, 31, 0));
			}
		}
	}
	glEnd2D();
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