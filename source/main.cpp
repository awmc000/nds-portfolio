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
 * Simple box, triangle, and putpixel demo
*/
void simple(int frame);

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
		simple(frame);
		glFlush(0);
		swiWaitForVBlank();
		scanKeys();
		int pressed = keysDown();
		if(pressed & KEY_START) break;
	}

}

void simple(int frame)
{

	// set up GL2D for 2d mode
	glBegin2D();

		// Do some funky color cycling
		int red = abs(sinLerp(frame*220)*31) >> 12 ;
		int green = abs(sinLerp(frame*140)*31) >> 12 ;
		int blue = abs(sinLerp(frame*40)*31) >> 12 ;
		
		// fill the whole screen with a gradient box
		glBoxFilledGradient( 0, 0, 255, 191,
							 RGB15( red,  green,  blue ),
							 RGB15(  blue, 31 - red,  green ),
							 RGB15( green,  blue, 31 - red ),
							 RGB15(  31 - green, red, blue )
						   );
		
		// draw a black box
		glBoxFilled( 200, 10,
					 250, 180,
					 RGB15(0,0,0)
				    );
		
		// draw a border around the black box
		glBox( 200, 10,
			   250, 180,
			   RGB15(0,31,0)
		     );
	
		// draw a triangle
		glTriangleFilled( 20, 100,
						  200, 30,
						  60, 40,
						  RGB15(31,0,31)
						);
	
		// draw a gradient triangle
		glTriangleFilledGradient( 20, 100,
								  200, 30,
								  60, 40,
								  RGB15(blue,red,green),
								  RGB15(green,blue, red),
								  RGB15(red,green,blue)
								);
		

		// translucent mode
		// Poly ID 1
		glPolyFmt(POLY_ALPHA(16) | POLY_CULL_NONE | POLY_ID(1));
		glBoxFilledGradient( 10, 50, 230, 150,
							 RGB15( green,  0,  0 ),
							 RGB15(  0, red,  0 ),
							 RGB15( 31,  0, blue ),
							 RGB15(  0, red, 31 )
						   );

		// translucent mode
		// Poly ID 2
		glPolyFmt(POLY_ALPHA(16) | POLY_CULL_NONE | POLY_ID(2));
		
		glTriangleFilledGradient( 70, 10,
								  20, 130,
								  230, 180,
								  RGB15(red,green,blue),
								  RGB15(blue,red,green),
								  RGB15(green,blue, red)
								);
		                       
		
		// restore to normal(solid) rendering
		glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID(3));
		
		// draw a circle using putpixel
		int i;
		for( i = 0; i < BRAD_PI*2; i += 256)
		{
			int x = (cosLerp(i) * 80 ) >> 12;
			int y = (sinLerp(i) * 70 ) >> 12;
			glPutPixel( HALF_WIDTH + x, HALF_HEIGHT + y, RGB15(red, green, blue) );
		}
			
	glEnd2D();
	

}