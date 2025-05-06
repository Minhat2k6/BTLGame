#include "Bomb.hpp"

Bomb::Bomb(float x, float y, SDL_Texture* tex)
    : x(x), y(y), texture(tex)
{
}

void Bomb::update(float deltaTime)
{
    y += speedY * deltaTime;
}

void Bomb::render(SDL_Renderer* renderer)
{
    SDL_Rect dst = { static_cast<int>(x), static_cast<int>(y), 32, 32 };
    SDL_RenderCopy(renderer, texture, NULL, &dst);
}

SDL_Rect Bomb::getRect()
{
    return { static_cast<int>(x), static_cast<int>(y), 32, 32 };
}
