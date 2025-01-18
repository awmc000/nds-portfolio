#include "raycaster.hpp"
#include <nds.h>
#include <gl2d.h>
#include <cmath>

void Raycaster::drawFrame() {
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