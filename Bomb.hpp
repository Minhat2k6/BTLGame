#pragma once
#include <SDL.h>

class Bomb {
public:
    Bomb(float x, float y, SDL_Texture* tex);

    void update(float deltaTime);
    void render(SDL_Renderer* renderer);
    SDL_Rect getRect();

private:
    float x, y;
    SDL_Texture* texture;
    float speedY = 100.0f;
};
