#include <SDL2/SDL.h>

class display
{
public:

	static void drawPixel(SDL_Surface *ds, short x, short y, int color){
		SDL_Rect p;
		p.x = x * 10;
		p.y = y * 10;
		p.w = 10;
		p.h = 10;
		SDL_FillRect(ds, &p, color);
	}
	static void drawScreen(SDL_Surface *ds, unsigned char gfx[64][32]){
		for (int y = 0; y < 32; ++y){
			for (int x = 0; x < 64; ++x){
				drawPixel(ds, x, y, (gfx[x][y] == 1) ? 0xFFFFFF : 0x00);
			}
		}
	}
};