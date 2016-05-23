#include "chip8.h"
#include "display.h"
#include <cstdio>
#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>

using namespace std;
chip8 *chip = new chip8();

const int winWidth = 640;
const int winHeight = 320;
const char* title = "Chip8 Emu";
bool running = true;

int main(int argc, char const *argv[])
{
	SDL_Window* emuWindow;
	SDL_Surface *screen;

	string romLocation;
	if (argv[1] == NULL){
		printf("Rom Location: ");
		cin >> romLocation;
	}
	else{
		romLocation = argv[1];
	}
	
	const char* op = romLocation.c_str();
	ifstream file(op, ifstream::in);
	file.seekg(0, ios::end);
	int romSize = file.tellg();

	char rom[romSize];
	file.seekg(0, ios::beg);
	file.read(rom, romSize);
	

	chip->init();
	chip->loadRom(rom, romSize);

	SDL_Init(SDL_INIT_VIDEO);
	emuWindow = SDL_CreateWindow(title, 0, 0, winWidth, winHeight, SDL_WINDOW_OPENGL);
	screen = SDL_GetWindowSurface(emuWindow);
	SDL_Event event;
	while(running){
		SDL_UpdateWindowSurface(emuWindow);
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				running = false;
			}
			else
			{
				switch(event.type){
					case SDL_KEYDOWN:
						switch(event.key.keysym.sym){
							case SDLK_1:
								chip->keys[0] = 1;
								break;
							case SDLK_2:
								chip->keys[1] = 1;
								break;
							case SDLK_3:
								chip->keys[2] = 1;
								break;
							case SDLK_4:
								chip->keys[3] = 1;
								break;
							case SDLK_q:
								chip->keys[4] = 1;
								break;
							case SDLK_w:
								chip->keys[5] = 1;
								break;
							case SDLK_e:
								chip->keys[6] = 1;
								break;
							case SDLK_r:
								chip->keys[7] = 1;
								break;
							case SDLK_a:
								chip->keys[8] = 1;
								break;
							case SDLK_s:
								chip->keys[9] = 1;
								break;
							case SDLK_d:
								chip->keys[10] = 1;
								break;
							case SDLK_f:
								chip->keys[11] = 1;
								break;
							case SDLK_z:
								chip->keys[12] = 1;
								break;
							case SDLK_x:
								chip->keys[13] = 1;
								break;
							case SDLK_c:
								chip->keys[14] = 1;
								break;
							case SDLK_v:
								chip->keys[15] = 1;
								break;							
						}
					break;
					case SDL_KEYUP:
						switch(event.key.keysym.sym){
							case SDLK_1:
								chip->keys[0] = 0;
								break;
							case SDLK_2:
								chip->keys[1] = 0;
								break;
							case SDLK_3:
								chip->keys[2] = 0;
								break;
							case SDLK_4:
								chip->keys[3] = 0;
								break;
							case SDLK_q:
								chip->keys[4] = 0;
								break;
							case SDLK_w:
								chip->keys[5] = 0;
								break;
							case SDLK_e:
								chip->keys[6] = 0;
								break;
							case SDLK_r:
								chip->keys[7] = 0;
								break;
							case SDLK_a:
								chip->keys[8] = 0;
								break;
							case SDLK_s:
								chip->keys[9] = 0;
								break;
							case SDLK_d:
								chip->keys[10] = 0;
								break;
							case SDLK_f:
								chip->keys[11] = 0;
								break;
							case SDLK_z:
								chip->keys[12] = 0;
								break;
							case SDLK_x:
								chip->keys[13] = 0;
								break;
							case SDLK_c:
								chip->keys[14] = 0;
								break;
							case SDLK_v:
								chip->keys[15] = 0;
								break;						
						}
					break;
				}
			}
		}
		chip->processOpcode();
		if (chip->drawFlag == true)
		{
			display::drawScreen(screen, chip->gfx);
			chip->drawFlag = false;
		}
	}
	SDL_FreeSurface(screen);
	SDL_DestroyWindow(emuWindow);
	SDL_Quit();
	return 0;
}
