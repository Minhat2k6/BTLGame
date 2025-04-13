#pragma once
#include <SDL.h>
#include <SDL2/SDL_image.h>

class Enemy {
public:
    Enemy(float p_x, float p_y, SDL_Texture* p_tex, int frameW, int frameH);
    void update(float deltaTime);
    void render(SDL_Renderer* renderer);
    
    float getX();
    float getY();
    SDL_Texture* getTex();
    SDL_Rect getCurrentFrame();

private:
    float x, y;
    float speed;
    SDL_Texture* tex;
    SDL_Rect currentFrame;

    int frameIndex;
    int frameDelay;
    static const int maxFrames = 4; 
};
