#pragma once
#include "globals.hpp"
#include "systems.hpp"

class Level
{
public:
	Level(int num, const char* path);
	~Level() {}	
};

Level::Level(int levelnum, const char* path)
{
	// Get file descriptor
	int fd = open(path, O_RDONLY);
	if (fd == -1) {
		auto& label = manager->AddEntity();
		label.AddComponent<TextComponent>("You win!", 80, POS_CENTER, POS_CENTER);
		return;
	}

	// Initialize variables
	std::vector<std::vector<int>> map(40, std::vector<int>(40, 0));
	char buf[1];
	int count = 0;
	int res;
	dir = DIR_NONE;
	
	// Read the file into the 2D vector `map`
	while ((res = read(fd, buf, 1)) != 0) {
		if (buf[0] == '\n')
			continue;		
		if (buf[0] == '1')
			map[count/40][count%40] = TILE_WALL;
		if (buf[0] == '2')
			map[count/40][count%40] = TILE_SPIKE;
		if (buf[0] == '3')
			map[count/40][count%40] = TILE_SPAWN;
		if (buf[0] == '4')
			map[count/40][count%40] = TILE_COIN;
		if (buf[0] == '5')
			map[count/40][count%40] = TILE_END;
		count++;
	}

	// Create the entities in the ECS
	for (int i = 0; i < 40; i++) {
		for (int j = 0; j < 40; j++) {
			if (map[i][j] == TILE_WALL) {
				auto& wall = manager->AddEntity();
				wall.name = "wall";
				wall.AddComponent<RenderComponent>(0, 0, 0);
				wall.AddComponent<TransformComponent>(i*40, j*40, 40, 40);
				wall.AddComponent<PhysicsComponent>(false);
			} else if (map[i][j] == TILE_SPIKE) {
				auto& spike = manager->AddEntity();
				spike.name = "spike";
				spike.AddComponent<RenderComponent>(255, 107, 107);
				spike.AddComponent<TransformComponent>(i*40, j*40, 40, 40);
				spike.AddComponent<PhysicsComponent>(false);
			} else if (map[i][j] == TILE_SPAWN) {
				auto& player = manager->AddEntity();
				player.name = "player";
				player.AddComponent<RenderComponent>(64, 0, 128);
				player.AddComponent<TransformComponent>(i*40, j*40, 40, 40);
				player.AddComponent<PhysicsComponent>();
			} else if (map[i][j] == TILE_END) {
				auto& end = manager->AddEntity();
				end.name = "end";
				end.AddComponent<RenderComponent>(32, 178, 170);
				end.AddComponent<TransformComponent>(i*40, j*40, 40, 40);
				end.AddComponent<PhysicsComponent>(false);
			}
		}
	}

	// Level title
	auto& label = manager->AddEntity();
	std::string leveltext = "Level " + std::to_string(levelnum);
	label.AddComponent<TextComponent>(leveltext, 80, POS_CENTER, POS_TOP);
}
