#include <iostream>
#include <SDL.h>
#include<SDL2/SDL_image.h>

#include "RenderWindow.hpp"
#include "Entity.hpp"

using namespace std;

 int main(int argc, char* argv[])
 {
 if(SDL_Init(SDL_INIT_VIDEO)>0){
 	cout << SDL_GetError()<<endl;
 	
 }
 if(!(IMG_Init(IMG_INIT_PNG))){
 		cout << SDL_GetError()<<endl;
 }
 RenderWindow window0("GAME HAY", 1000, 600);
 SDL_Texture* knightTexture = window0.loadTexture("hulking_knight.png");
 SDL_Texture* backgroundTexture = window0.loadTexture("background.png");
 
 Entity knight(100, 100, knightTexture);
 
 
bool gameRunning = true;

	SDL_Event event;

	Uint32 lastTime = SDL_GetTicks();
    bool spacePressed = false;
   while (gameRunning)
    {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        
        // Reset tr?ng thái di chuy?n m?i frame
        knight.isMoving = false;
        
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) gameRunning = false;
            
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE && !spacePressed) {
                knight.jump(10); // l?c nh?y hi?n gi? du?c gi?m xu?ng cho h?p lý
                spacePressed = true;
            }
            
            if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_SPACE) {
                spacePressed = false;
            }
        }
        
        const Uint8* keystates = SDL_GetKeyboardState(NULL);
        float speed = 200.0f;
        if (keystates[SDL_SCANCODE_RIGHT]) { 
		    knight.move(speed * deltaTime, 0);
		    knight.isMoving = true;
		    knight.facingLeft = false;
   } 
       if (keystates[SDL_SCANCODE_LEFT]) { 
		    knight.move(-speed * deltaTime, 0);
		    knight.isMoving = true;
		    knight.facingLeft = true;
	}
        knight.applyGravity(30 * deltaTime);
        knight.update();

        window0.clear();
        window0.render(backgroundTexture);
        window0.render(knight);
        window0.display();
    }

    window0.cleanUp();
    SDL_Quit();
    return 0;
 }
