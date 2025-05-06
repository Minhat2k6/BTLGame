#include "Enemy.hpp"
#include<iostream>
using namespace std;
Enemy::Enemy(float p_x, float p_y, SDL_Texture* p_tex, int frameW, int frameH)
    : x(p_x), y(p_y), tex(p_tex), speed(40.0f), frameIndex(0), frameDelay(0)
{
    currentFrame.x = 0;
    currentFrame.y = 0;
    currentFrame.w = frameW;
    currentFrame.h = frameH;
}

void Enemy::update(float deltaTime) {
    x += speed * deltaTime;

    if (x > 1000) {
        x = -currentFrame.w;
    }

    frameDelay++;
    if (frameDelay >= 10) {
        frameDelay = 0;
        frameIndex = (frameIndex + 1) % maxFrames;
        currentFrame.x = frameIndex * currentFrame.w;
    }
}

void Enemy::render(SDL_Renderer* renderer) {
    SDL_Rect dst;
    dst.x = static_cast<int>(x);
    dst.y = static_cast<int>(y);
    dst.w = currentFrame.w * 2; 
    dst.h = currentFrame.h * 2;

    SDL_RenderCopy(renderer, tex, &currentFrame, &dst);
}
float Enemy::getX() {
    return x;
}

float Enemy::getY() {
    return y;
}

SDL_Texture* Enemy::getTex() {
    return tex;
}

SDL_Rect Enemy::getCurrentFrame() {
    return currentFrame;
}
