#include <SDL2/SDL.h>
#include <iostream>
#include <fstream>
#include <vector>

#define WIN_SIZE 1600
#define BOX_SIZE 40
#define BOX_COUNT WIN_SIZE/BOX_SIZE

SDL_Window* window;
SDL_Renderer* renderer;
bool running = true;
std::vector<std::vector<int>> map(BOX_COUNT, std::vector<int>(BOX_COUNT, 0));

bool clicked = false;
int mode = 1;

void AddBox(int x, int y, int type)
{
	map[x][y] = type;
}

void DrawLines()
{
	SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
	for (int i = 0; i < BOX_COUNT-1; i++) {
			SDL_Rect rect;
			
			rect.x = BOX_SIZE + i*BOX_SIZE-2;
			rect.y = 0;
			rect.w = 2;
			rect.h = WIN_SIZE;
			SDL_RenderFillRect(renderer, &rect);

			rect.x = 0;
			rect.y = BOX_SIZE + i*BOX_SIZE-2;
			rect.w = WIN_SIZE;
			rect.h = 2;
			SDL_RenderFillRect(renderer, &rect);
	}
}
	
void DrawBoxes()
{
	for (int i = 0; i < BOX_COUNT; i++) {
		for (int j = 0; j < BOX_COUNT; j++) {
			switch (map[i][j]) {
			case 0:
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				break;
			case 1:
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				break;
			case 2:
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
				break;
			case 3:
				SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
				break;
			case 4:
				SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
				break;
			case 5:
				SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
				break;
			}

			SDL_Rect rect;
			rect.x = i*BOX_SIZE;
			rect.y = j*BOX_SIZE;
			rect.w = BOX_SIZE;
			rect.h = BOX_SIZE;
			SDL_RenderFillRect(renderer, &rect);
		}
	}
}

void update()
{
	SDL_Event e;
    while(SDL_PollEvent(&e)){
        switch(e.type){
		case SDL_QUIT:
			running = false;
			break;

		case SDL_KEYDOWN:
			switch (e.key.keysym.scancode) {
			case SDL_SCANCODE_0:
				mode = 0;
				break;
			case SDL_SCANCODE_1:
				mode = 1;
				break;
			case SDL_SCANCODE_2:
				mode = 2;
				break;
			case SDL_SCANCODE_3:
				mode = 3;
				break;
			case SDL_SCANCODE_4:
				mode = 4;
				break;
			case SDL_SCANCODE_5:
				mode = 5;
				break;
			}
			break;
			
		case SDL_MOUSEBUTTONDOWN:
			if (e.button.button == SDL_BUTTON_LEFT)
				clicked = true;
			AddBox(e.button.x/BOX_SIZE, e.button.y/BOX_SIZE, mode);
			break;
		case SDL_MOUSEBUTTONUP:
			if (e.button.button == SDL_BUTTON_LEFT)
				clicked = false;
			break;
		case SDL_MOUSEMOTION:
			if (clicked)
				AddBox(e.button.x/BOX_SIZE, e.button.y/BOX_SIZE, mode);
			break;
        }
    }

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	DrawBoxes();
	DrawLines();
	SDL_RenderPresent(renderer);
}

int main()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("Tilemap Maker", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_SIZE, WIN_SIZE, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, 0);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
	
	while (running) {
		update();
		SDL_Delay(10);
	}

	std::ofstream file;
	file.open("map.txt");
	for (int i = 0; i < BOX_SIZE; i++) {
		for (int j = 0; j < BOX_SIZE; j++) {
			file << map[i][j];
		}
		file << std::endl;
	}
	file.close();
	
	return 0;
}
