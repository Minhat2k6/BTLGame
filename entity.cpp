#include "Entity.hpp"
#include <SDL.h>
#include <SDL2/SDL_image.h>

Entity::Entity(float p_x, float p_y, SDL_Texture* p_tex)
    : x(p_x), y(p_y), tex(p_tex), frameIndex(0), frameDelay(0), velY(0), onGround(false), touchingWall(false)
{
    currentFrame.x = 0;
    currentFrame.y = 0;
    currentFrame.w = 64;
    currentFrame.h = 64;
}

Entity::Entity(float p_x, float p_y, SDL_Texture* p_tex, int frameW, int frameH)
    : x(p_x), y(p_y), tex(p_tex), frameIndex(0), frameDelay(0), velY(0), onGround(false), touchingWall(false)
{
    currentFrame.x = 0;
    currentFrame.y = 0;
    currentFrame.w = frameW;
    currentFrame.h = frameH;
}

float Entity::getX()
{
	return x;
}
float Entity::getY()
{
	return y;
}

SDL_Texture* Entity::getTex()
{
	return tex;
}
int Entity::getframeIndex(){
	return frameIndex;
}
int Entity::getframeDelay(){
	return frameDelay;
}

SDL_Rect Entity::getCurrentFrame()
{
	return currentFrame;
}
void Entity::applyGravity(float gravity) {
    if (!onGround) velY += gravity;
}

void Entity::jump(float force) {
	if (onGround || touchingWall) {
		velY = -force;
		onGround = false;
	}
}

void Entity::update() {
	y += velY;

	int groundY = 550;
	if (y + currentFrame.h >= groundY) {
		y = groundY - currentFrame.h;
		velY = 0;
		onGround = true;
	} else {
		onGround = false;
	}

	if (x <= -40) {
        x = 0;
        touchingWall = true;
    } else if (x + currentFrame.w * 2 >= 1040) { 
        x = 1040 - currentFrame.w * 2;
        touchingWall = true;
    } else {
        touchingWall = false;
    }

	if (isMoving) {
        frameDelay++;
        if (frameDelay >= 10) {
            frameDelay = 0;
            frameIndex++;
            
            int textureW = 0, textureH = 0;
            SDL_QueryTexture(tex, NULL, NULL, &textureW, &textureH);
            int maxFrameInImage = textureW / currentFrame.w;

            frameIndex %= maxFrameInImage;
            currentFrame.x = frameIndex * currentFrame.w;
        }
    } else {
        frameIndex = 0;
        currentFrame.x = 0;
    }
}



void Entity::move(float dx, float dy) {
    x += dx;
    y += dy;
    isMoving = (dx != 0);
}

