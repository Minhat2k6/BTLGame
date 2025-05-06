#include <SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "RenderWindow.hpp"
 using namespace std;
 
RenderWindow::RenderWindow(const char* p_title, int p_w, int p_h)
	:window(NULL), renderer(NULL)
{
	window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_SHOWN);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}
SDL_Texture* RenderWindow::loadTexture(const char* p_filePath)// hàm ki?u SDL_Texture
{
	SDL_Surface* surface = IMG_Load(p_filePath);
    
    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 255, 255));

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    return texture;
}
void RenderWindow::cleanUp()
{
	SDL_DestroyWindow(window);
}
void RenderWindow::clear()
{
	SDL_RenderClear(renderer);
}

void RenderWindow::render(Entity& p_entity)
{
	SDL_Rect src; 
	src.x = p_entity.getCurrentFrame().x;
	src.y = p_entity.getCurrentFrame().y;
	src.w = p_entity.getCurrentFrame().w;
	src.h = p_entity.getCurrentFrame().h;

	SDL_Rect dst;
    dst.x = static_cast<int>(p_entity.getX()) ;
    dst.y = static_cast<int>(p_entity.getY()) ;
	dst.w = p_entity.getCurrentFrame().w *2 ;
	dst.h = p_entity.getCurrentFrame().h *2 ;
	SDL_RendererFlip flip = p_entity.facingLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    SDL_RenderCopyEx(renderer, p_entity.getTex(), &src, &dst, 0, nullptr, flip);
}

void RenderWindow::render(SDL_Texture* tex)
{
    SDL_RenderCopy(renderer, tex, NULL, NULL); 
}


void RenderWindow::display()
{
	SDL_RenderPresent(renderer);
}
SDL_Renderer* RenderWindow::getRenderer() {
    return renderer;
}

SDL_Window* RenderWindow::getWindow() {
    return window;
}

