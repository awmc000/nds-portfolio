/*---------------------------------------------------------------------------------

	Basic template code for starting a DS app

---------------------------------------------------------------------------------*/
#include <nds.h>
#include <gl2d.h>
#include <stdio.h>

// Relminator's constants
#define HALF_WIDTH (SCREEN_WIDTH/2)
#define HALF_HEIGHT (SCREEN_HEIGHT/2)
#define BRAD_PI (1 << 14)

/**
 * NOT BY ME! By Relminator
 * Radially displaced pixels demo
*/
void pixels(int frame);

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------

	videoSetMode(MODE_5_3D);
	glScreen2D();

	consoleDemoInit();
	iprintf("Hello World!");

	int frame = 0;

	while(1) {
		frame++;
		pixels(frame);
		glFlush(0);
		swiWaitForVBlank();
		scanKeys();
		int pressed = keysDown();
		if(pressed & KEY_START) break;
	}

}

void pixels(int frame)
{
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