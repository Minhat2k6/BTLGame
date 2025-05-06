#pragma once
#include <SDL.h>
#include <vector>
#include "Entity.hpp"
#include "Bomb.hpp"

class FlyingEnemy : public Entity {
public:
    FlyingEnemy(float x, float y, SDL_Texture* tex, SDL_Texture* bombTex);
   
    FlyingEnemy(float x, float y, SDL_Texture* tex, int frameW, int frameH);

    void update(float deltaTime);
    void render(SDL_Renderer* renderer);

    void dropBomb(SDL_Texture* bombTex, std::vector<Bomb>& bombs);

private:
    float speedX;
    float maxX, minX;
    bool movingLeft;

    float bombCooldown;
    const float bombDropDelay = 5.0f;
    SDL_Texture* bombTexture; 
};
