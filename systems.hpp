#pragma once
#include "globals.hpp"

struct TransformComponent : public Component
{
    float x, y, w, h;
    TransformComponent(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {};
};

struct RenderComponent : public Component
{
    bool render = true;
    int r = 255;
    int g = 0;
    int b = 0;
    RenderComponent() {}
    RenderComponent(bool render) : render(render) {};
    RenderComponent(int r, int g, int b) : r(r), g(g), b(b) {};
};

struct PhysicsComponent : public Component
{
	bool moving = true;
    float xvel = 0;
	float yvel = 0;
    PhysicsComponent() {}
    PhysicsComponent(bool moving) : moving(moving) {};
};

struct TextComponent : public Component
{
	#define POS_LEFT 0
	#define POS_CENTER 1
	#define POS_RIGHT 2
	#define POS_TOP 3
	#define POS_BOTTOM 4
	int hPosition;
	int vPosition;
	TTF_Font* font;
	SDL_Surface* surface;
	SDL_Texture* message;

	TextComponent(std::string text, int size, int hp, int vp)
	{
		font = TTF_OpenFont("FreeMono.ttf", size);
		SDL_Color black = {0, 0, 0};
		surface = TTF_RenderText_Solid(font, text.c_str(), black);
		message = SDL_CreateTextureFromSurface(renderer, surface);
		hPosition = hp;
		vPosition = vp;
	}
};

struct TextRenderSystem : public System
{
	int padding = 10;
	TextRenderSystem() { AddComponents<TextComponent>(); }
	void Update(double deltatime, std::vector<Entity*> entities)
	{
		for (Entity* e : entities) {
			auto text = e->GetComponent<TextComponent>();

			// Initialize rectangle
			SDL_Rect rect;
			rect.w = text->surface->w;
			rect.h = text->surface->h;

			// Find horizontal position
			switch (text->hPosition) {
			case POS_LEFT:
				rect.x = padding;
				break;
			case POS_CENTER:
				rect.x = WIN_WIDTH/2 - rect.w/2;
				break;
			case POS_RIGHT:
				rect.x = WIN_WIDTH - rect.w - padding;
				break;
			}

			// Find vertical position
			switch (text->vPosition) {
			case POS_TOP:
				rect.y = padding;
				break;
			case POS_CENTER:
				rect.y = WIN_HEIGHT/2 - rect.h/2;
				break;
			case POS_BOTTOM:
				rect.y = WIN_HEIGHT - rect.h - padding;
				break;
			}

			SDL_RenderCopy(renderer, text->message, NULL, &rect);
		}
	}
};
	
struct RenderSystem : public System
{
    RenderSystem() { AddComponents<TransformComponent, RenderComponent>(); }
    void Update(double deltatime, std::vector<Entity*> entities)
    {
        for (Entity* e : entities) {
	        auto r = e->GetComponent<RenderComponent>();
            if (!r->render) continue;
            auto t = e->GetComponent<TransformComponent>();
            SDL_SetRenderDrawColor(renderer, r->r, r->g, r->b, 255);
			
            SDL_Rect rect;
            rect.x = (int)t->x;
            rect.y = (int)t->y;
            rect.w = (int)t->w;
            rect.h = (int)t->h;
            SDL_RenderFillRect(renderer, &rect);

			if (e->name == "player") {
				rect.x += 10;
				rect.y += 10;
				rect.w -= 20;
				rect.h -= 20;
				SDL_SetRenderDrawColor(renderer, 178, 153, 204, 255);
				SDL_RenderFillRect(renderer, &rect);
			}
        }
    }
};

bool AABB(Entity* e1, Entity* e2)
{
    auto t1 = e1->GetComponent<TransformComponent>();
    auto t2 = e2->GetComponent<TransformComponent>();
    return ((int)t1->x < (t2->x + t2->w) &&
            (int)t2->x < (t1->x + t1->w) &&
            (int)t1->y < (t2->y + t2->h) &&
            (int)t2->y < (t1->y + t1->h));
}

struct PhysicsSystem : public System
{
	float gravity = 0.01;
    PhysicsSystem() { AddComponents<TransformComponent, PhysicsComponent>(); }
    void Update(double deltatime, std::vector<Entity*> entities)
    {	   
     	for (Entity* e : entities) {
            auto trans = e->GetComponent<TransformComponent>();
            auto phys = e->GetComponent<PhysicsComponent>();

			// Apply gravity
            if (phys->moving) {
				if (dir == DIR_UP)
					phys->yvel -= gravity * deltatime;
				else if (dir == DIR_DOWN)
					phys->yvel += gravity * deltatime;
				else if (dir == DIR_LEFT)
					phys->xvel -= gravity * deltatime;
				else if (dir == DIR_RIGHT)
					phys->xvel += gravity * deltatime;
			} else continue;
			
            // X axis movement
            trans->x += phys->xvel;
            for (Entity* e2 : entities) {
                if (e == e2) continue;
				if (e->name == "player" && e2->name == "player") continue;

				auto trans2 = e2->GetComponent<TransformComponent>();
                if (AABB(e, e2)) {				   
					if (e->name == "spike" || e2->name == "spike")
						lost = true;
					else if (e->name == "end" || e2->name == "end")
						beat = true;
                    if (phys->xvel > 0)
                        trans->x = trans2->x - trans->w;
                    else if (phys->xvel < 0)
                        trans->x = trans2->x + trans2->w;
                    phys->xvel = 0;
				}
            }
			
			// Y axis movement
            trans->y += phys->yvel;
            for (Entity* e2 : entities) {
                if (e == e2) continue;
				if (e->name == "player" && e2->name == "player") continue;
				
                auto trans2 = e2->GetComponent<TransformComponent>();
                if (AABB(e, e2)) {
					if (e->name == "spike" || e2->name == "spike")
						lost = true;
					else if (e->name == "end" || e2->name == "end")
						beat = true;
                    if (phys->yvel > 0)
						trans->y = trans2->y - trans->h;
                    else if (phys->yvel < 0)
                        trans->y = trans2->y + trans2->h;
                    phys->yvel = 0;
                }
            }
        }
    }
};
