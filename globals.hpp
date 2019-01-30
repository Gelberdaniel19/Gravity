#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "ECS.hpp"

#define WIN_WIDTH 1600
#define WIN_HEIGHT 1600

#define DIR_NONE 0
#define DIR_UP 1
#define DIR_DOWN 2
#define DIR_LEFT 3
#define DIR_RIGHT 4
int dir = DIR_NONE;

#define TILE_VOID 0
#define TILE_WALL 1
#define TILE_SPIKE 2
#define TILE_SPAWN 3
#define TILE_COIN 4
#define TILE_END 5

SDL_Window* window;
SDL_Renderer* renderer;

bool running = true;
bool lost = false;
bool beat = false;

AutoManager manager(new EntityManager());
