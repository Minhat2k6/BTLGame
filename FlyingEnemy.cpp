#include "FlyingEnemy.hpp"

FlyingEnemy::FlyingEnemy(float x, float y, SDL_Texture* tex, SDL_Texture* bombTex)
    : Entity(x, y, tex), speedX(100.0f), minX(x - 150), maxX(x + 150), 
      movingLeft(false), bombCooldown(0.0f), bombTexture(bombTex)
{
}

// Thêm constructor th? hai phù h?p v?i cách g?i trong main
FlyingEnemy::FlyingEnemy(float x, float y, SDL_Texture* tex, int frameW, int frameH)
    : Entity(x, y, tex, frameW, frameH), speedX(100.0f), minX(x - 150), maxX(x + 150),
      movingLeft(false), bombCooldown(0.0f), bombTexture(nullptr)
{
}

void FlyingEnemy::update(float deltaTime)
{
    if (movingLeft) {
        move(-speedX * deltaTime, 0);
        if (getX() <= minX) movingLeft = false;
    } else {
        move(speedX * deltaTime, 0);
        if (getX() >= maxX) movingLeft = true;
    }

    bombCooldown -= deltaTime;
}

void FlyingEnemy::render(SDL_Renderer* renderer)
{
    SDL_Rect src = getCurrentFrame();
    SDL_Rect dst = {
        static_cast<int>(getX()),
        static_cast<int>(getY()),
        src.w * 2,
        src.h * 2
    };
    SDL_RenderCopy(renderer, getTex(), &src, &dst);
}

void FlyingEnemy::dropBomb(SDL_Texture* bombTex, std::vector<Bomb>& bombs)
{
    if (bombCooldown <= 0.0f) {
        SDL_Texture* texToUse = bombTexture ? bombTexture : bombTex;
        bombs.emplace_back(getX() + 20, getY() + 40, texToUse); 
        bombCooldown = bombDropDelay;
    }
}
