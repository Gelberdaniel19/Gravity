#include "globals.hpp"
#include "level.hpp"

void HandleInput()
{
	SDL_Event e;
    while(SDL_PollEvent(&e)){
        switch(e.type){
		case SDL_QUIT:
			running = false;
			break;

		case SDL_KEYDOWN:
			switch (e.key.keysym.sym) {
			case SDLK_UP:
			case SDLK_w:
				dir = DIR_UP;
				break;
			case SDLK_LEFT:
			case SDLK_a:
				dir = DIR_LEFT;
				break;
			case SDLK_DOWN:
			case SDLK_s:
				dir = DIR_DOWN;
				break;
			case SDLK_RIGHT:
			case SDLK_d:
				dir = DIR_RIGHT;
				break;
			}
			break;		   
        }
    }
}

int main()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	window = SDL_CreateWindow("Gravity", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderPresent(renderer);

	manager->AddSystem<TextRenderSystem>();
	manager->AddSystem<RenderSystem>();
	manager->AddSystem<PhysicsSystem>();

	auto& title = manager->AddEntity();
	title.AddComponent<TextComponent>("Gravity.", 120, POS_CENTER, POS_CENTER);
	SDL_RenderClear(renderer);
	manager->Update(0);
	SDL_RenderPresent(renderer);
	SDL_Delay(3000);
	title.Destroy();
	
	int levelnum = 1;
	std::string levelstr = "levels/" + std::to_string(levelnum);
	Level* level = new Level(levelnum, levelstr.c_str());
	
	while (running) {
		HandleInput();
		SDL_RenderClear(renderer);
		manager->Update(10);
		SDL_RenderPresent(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_Delay(10);
		if (beat) {
			beat = false;
			levelnum++;
			manager->Purge();
			levelstr = "levels/" + std::to_string(levelnum);
			delete level;
			level = new Level(levelnum, levelstr.c_str());
		}
		if (lost) {
			lost = false;			
			manager->Purge();
			delete level;
			level = new Level(levelnum, levelstr.c_str());
		}
	}
	
	return 0;
}
