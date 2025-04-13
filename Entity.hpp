#pragma once
#include <SDL.h>
#include <SDL2/SDL_image.h>

class Entity
{
public:
	Entity(float p_x, float p_y, SDL_Texture* p_tex);
	Entity(float p_x, float p_y, SDL_Texture* p_tex, int frameW, int frameH);
	float getX();
	float getY();
	int getframeIndex();
	int getframeDelay();
	SDL_Texture* getTex();
	SDL_Rect getCurrentFrame();
	void update();
	void move(float dx, float dy);
	
	void applyGravity(float gravity);
    void jump(float force);
    
    float velY;
    bool onGround;
    bool touchingWall;
    
    bool isMoving = false;
private:
	float x, y;
	SDL_Rect currentFrame;
	SDL_Texture* tex;
	
	int frameIndex;
    int frameDelay;
    const int maxFrames = 4;
};
