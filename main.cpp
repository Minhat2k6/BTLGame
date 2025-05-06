#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "RenderWindow.hpp"
#include "Entity.hpp"
#include "Enemy.hpp"
#include "FlyingEnemy.hpp"
#include "Bomb.hpp"

using namespace std;


enum GameState {
    MENU,
    GAME,
    INSTRUCTIONS,
    GAME_OVER
};

int main(int argc, char* argv[])
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        cout << "SDL failed to initialize: " << SDL_GetError() << endl;
        return 1;
    }
    if(!(IMG_Init(IMG_INIT_PNG))){
        cout << "SDL_image failed to initialize: " << SDL_GetError() << endl;
        return 1;
    }

    if (TTF_Init() == -1) {
        cout << "SDL_ttf failed to initialize: " << TTF_GetError() << endl;
        return 1;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        cout << "SDL_mixer failed to initialize: " << Mix_GetError() << endl;
        return 1;
    }
    
    Mix_Chunk* hurtSound = Mix_LoadWAV("dame.wav");
    Mix_Chunk* explosionSound = Mix_LoadWAV("bomb.wav");
    Mix_Music* gameOverMusic = Mix_LoadMUS("gameover.mp3");
    
    if (!hurtSound || !explosionSound || !gameOverMusic) {
        cout << "Failed to load sound effects: " << Mix_GetError() << endl;
    }
    bool gameOverMusicPlaying = false;
    
    RenderWindow window0("GAME HAY", 1000, 600);
    
   
    SDL_Texture* knightTexture = window0.loadTexture("hulking_knight.png");
    SDL_Texture* backgroundTexture = window0.loadTexture("background.png");
    SDL_Texture* slimeTexture = window0.loadTexture("slime.png");
    SDL_Texture* heartTexture = window0.loadTexture("heart.png");
    SDL_Texture* flyingEnemyTex = window0.loadTexture("chit.png");
    SDL_Texture* bombTex = window0.loadTexture("Bomp.png");
    SDL_Texture* replayButtonTexture = window0.loadTexture("replay.png");
    
    SDL_Texture* menuBackgroundTexture = window0.loadTexture("menu.png");
   
    if (!menuBackgroundTexture) {
        menuBackgroundTexture = backgroundTexture;
    }
    
    if (!replayButtonTexture) {
        cout << "Failed to load replay button texture: " << SDL_GetError() << endl;
    }
    
    TTF_Font* font = TTF_OpenFont("impact.ttf", 24);
    TTF_Font* titleFont = TTF_OpenFont("impact.ttf", 48); 
    if (!font) {
        cout << "Failed to load font: " << TTF_GetError() << endl;
        return 1;
    }
    if (!titleFont) {
        titleFont = font; 
    }
    
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Color highlightColor = {255, 255, 0, 255}; 
    
    FlyingEnemy flyingEnemy(300, 100, flyingEnemyTex, bombTex);
    
    Entity knight(100, 100, knightTexture);
    
    
    SDL_Rect playButtonRect = {400, 250, 200, 50};
    SDL_Rect instructionsButtonRect = {400, 320, 200, 50};
    SDL_Rect backButtonRect = {400, 500, 200, 50};
    SDL_Rect replayButtonRect = {400, 350, 200, 50};
    
  
    GameState currentState = MENU;
    
    vector<Enemy> enemies;
    vector<FlyingEnemy> flyers;
    vector<Bomb> bombs;
    
    
    auto resetGame = [&]() {
        enemies.clear();
        enemies.push_back(Enemy(-100, 530, slimeTexture, 32, 32));
        enemies.push_back(Enemy(-340, 530, slimeTexture, 32, 32));
        enemies.push_back(Enemy(-540, 530, slimeTexture, 32, 32));
        enemies.push_back(Enemy(-860, 530, slimeTexture, 32, 32));
        
        flyers.clear();
        flyers.push_back(FlyingEnemy(200, 100, flyingEnemyTex, bombTex));
        flyers.push_back(FlyingEnemy(600, 150, flyingEnemyTex, bombTex));
        flyers.push_back(FlyingEnemy(400, 120, flyingEnemyTex, bombTex));
        
        bombs.clear();
        
        knight.move(100 - knight.getX(), 100 - knight.getY());
        
        if (gameOverMusicPlaying) {
            Mix_HaltMusic();
            gameOverMusicPlaying = false;
        }
    };
    
   
    resetGame();
    
    int hearts = 3; 
    int score = 0;
    float scoreTimer = 0.0f;
    
    bool gameRunning = true;
    SDL_Event event;
    Uint32 lastTime = SDL_GetTicks();
    bool spacePressed = false;
    
    
    while (gameRunning)
    {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        
     
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        
        bool playButtonHovered = 
            mouseX >= playButtonRect.x && mouseX <= playButtonRect.x + playButtonRect.w &&
            mouseY >= playButtonRect.y && mouseY <= playButtonRect.y + playButtonRect.h;
        
        bool instructionsButtonHovered = 
            mouseX >= instructionsButtonRect.x && mouseX <= instructionsButtonRect.x + instructionsButtonRect.w &&
            mouseY >= instructionsButtonRect.y && mouseY <= instructionsButtonRect.y + instructionsButtonRect.h;
        
        bool backButtonHovered = 
            mouseX >= backButtonRect.x && mouseX <= backButtonRect.x + backButtonRect.w &&
            mouseY >= backButtonRect.y && mouseY <= backButtonRect.y + backButtonRect.h;
        
        bool replayButtonHovered = 
            mouseX >= replayButtonRect.x && mouseX <= replayButtonRect.x + replayButtonRect.w &&
            mouseY >= replayButtonRect.y && mouseY <= replayButtonRect.y + replayButtonRect.h;
        
        if (currentState == GAME) {
            knight.isMoving = false;
            
            scoreTimer += deltaTime;
            if (scoreTimer >= 3.5f) {
                score += 100;
                scoreTimer = 0.0f;
            }
        }
        
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) gameRunning = false;
            
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                switch (currentState) {
                    case MENU:
                        if (playButtonHovered) {
                            currentState = GAME;
                            resetGame();
                            hearts = 3;
                            score = 0;
                            scoreTimer = 0.0f;
                        } else if (instructionsButtonHovered) {
                            currentState = INSTRUCTIONS;
                        }
                        break;
                        
                    case INSTRUCTIONS:
                        if (backButtonHovered) {
                            currentState = MENU;
                        }
                        break;
                        
                    case GAME_OVER:
                        if (replayButtonHovered) {
                            currentState = GAME;
                            hearts = 3;
                            score = 0;
                            scoreTimer = 0.0f;
                            resetGame();
                        }
                        break;
                        
                    default:
                        break;
                }
            }
            
            if (currentState == GAME && event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE && !spacePressed) {
                knight.jump(13); 
                spacePressed = true;
            }
            
            if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_SPACE) {
                spacePressed = false;
            }
            
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                if (currentState == GAME) {
                    currentState = MENU;
                }
            }
        }
        
        if (currentState == GAME) {
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
            
            for (auto& enemy : enemies) {
                enemy.update(deltaTime);
                
                SDL_Rect knightRect = {
                    static_cast<int>(knight.getX()),
                    static_cast<int>(knight.getY()),
                    knight.getCurrentFrame().w,
                    knight.getCurrentFrame().h 
                };
                SDL_Rect enemyRect = {
                    static_cast<int>(enemy.getX()),
                    static_cast<int>(enemy.getY()),
                    enemy.getCurrentFrame().w * 2 - 36,
                    enemy.getCurrentFrame().h * 2 - 36
                };
                
                if (SDL_HasIntersection(&knightRect, &enemyRect)) {
                    if (hearts > 0) {
                        hearts--;
                        knight.move(40, 0); 
                        if (hurtSound) {
                            Mix_PlayChannel(-1, hurtSound, 0);
                        }
                        SDL_Delay(100); 
                        
                        if (hearts <= 0) {
                            currentState = GAME_OVER;
                            if (gameOverMusic && !gameOverMusicPlaying) {
                                Mix_PlayMusic(gameOverMusic, -1); 
                                gameOverMusicPlaying = true;
                            }
                        }
                    }
                }
            }
            
            for (auto& flyer : flyers) {
                flyer.update(deltaTime);
                flyer.dropBomb(bombTex, bombs);
            }
            
            for (size_t i = 0; i < bombs.size(); i++) {
                bombs[i].update(deltaTime);
                
                SDL_Rect knightRect = {
                    static_cast<int>(knight.getX()),
                    static_cast<int>(knight.getY()),
                    knight.getCurrentFrame().w-10,
                    knight.getCurrentFrame().h-10
                };
                SDL_Rect bombRect = bombs[i].getRect();
                bombRect.w -= 15;  
                bombRect.h -= 15;  
                bombRect.x += 5;  
                bombRect.y += 5;
                
                if (SDL_HasIntersection(&knightRect, &bombRect)) {
                    if (hearts > 0) {
                        hearts--;
                        if (explosionSound) {
                            Mix_PlayChannel(-1, explosionSound, 0);
                        }
                        bombs.erase(bombs.begin() + i); 
                        i--; 
                        SDL_Delay(100);
                        
                        if (hearts <= 0) {
                            currentState = GAME_OVER;
                            if (gameOverMusic && !gameOverMusicPlaying) {
                                Mix_PlayMusic(gameOverMusic, -1);
                                gameOverMusicPlaying = true;
                            }
                        }
                        continue;
                    }
                }
                
                if (bombRect.y > 600) {
                    bombs.erase(bombs.begin() + i);
                    i--; 
                }
            }
        }
        
 
        window0.clear();
        
        switch (currentState) {
            case MENU:
                window0.render(menuBackgroundTexture);
                
              
                {
                    string titleText = "KNIGHT'S ADVENTURE";
                    SDL_Surface* titleSurface = TTF_RenderText_Solid(titleFont, titleText.c_str(), textColor);
                    SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(window0.getRenderer(), titleSurface);
                    
                    SDL_Rect titleRect;
                    titleRect.x = (1000 - titleSurface->w) / 2;
                    titleRect.y = 100;
                    titleRect.w = titleSurface->w;
                    titleRect.h = titleSurface->h;
                    
                    SDL_RenderCopy(window0.getRenderer(), titleTexture, NULL, &titleRect);
                    SDL_FreeSurface(titleSurface);
                    SDL_DestroyTexture(titleTexture);
                }
                
         
                {
                    SDL_SetRenderDrawColor(window0.getRenderer(), 50, 50, 50, 255);
                    SDL_RenderFillRect(window0.getRenderer(), &playButtonRect);
                    
                    SDL_SetRenderDrawColor(window0.getRenderer(), 255, 255, 255, 255);
                    SDL_RenderDrawRect(window0.getRenderer(), &playButtonRect);
                    
                    string playText = "PLAY";
                    SDL_Surface* playSurface = TTF_RenderText_Solid(font, playText.c_str(), 
                                                                 playButtonHovered ? highlightColor : textColor);
                    SDL_Texture* playTexture = SDL_CreateTextureFromSurface(window0.getRenderer(), playSurface);
                    
                    SDL_Rect playTextRect;
                    playTextRect.x = playButtonRect.x + (playButtonRect.w - playSurface->w) / 2;
                    playTextRect.y = playButtonRect.y + (playButtonRect.h - playSurface->h) / 2;
                    playTextRect.w = playSurface->w;
                    playTextRect.h = playSurface->h;
                    
                    SDL_RenderCopy(window0.getRenderer(), playTexture, NULL, &playTextRect);
                    SDL_FreeSurface(playSurface);
                    SDL_DestroyTexture(playTexture);
                }
                
          
                {
                    SDL_SetRenderDrawColor(window0.getRenderer(), 50, 50, 50, 255);
                    SDL_RenderFillRect(window0.getRenderer(), &instructionsButtonRect);
                    
                    SDL_SetRenderDrawColor(window0.getRenderer(), 255, 255, 255, 255);
                    SDL_RenderDrawRect(window0.getRenderer(), &instructionsButtonRect);
                    
                    string instructionsText = "INSTRUCTIONS";
                    SDL_Surface* instructionsSurface = TTF_RenderText_Solid(font, instructionsText.c_str(), 
                                                                   instructionsButtonHovered ? highlightColor : textColor);
                    SDL_Texture* instructionsTexture = SDL_CreateTextureFromSurface(window0.getRenderer(), instructionsSurface);
                    
                    SDL_Rect instructionsTextRect;
                    instructionsTextRect.x = instructionsButtonRect.x + (instructionsButtonRect.w - instructionsSurface->w) / 2;
                    instructionsTextRect.y = instructionsButtonRect.y + (instructionsButtonRect.h - instructionsSurface->h) / 2;
                    instructionsTextRect.w = instructionsSurface->w;
                    instructionsTextRect.h = instructionsSurface->h;
                    
                    SDL_RenderCopy(window0.getRenderer(), instructionsTexture, NULL, &instructionsTextRect);
                    SDL_FreeSurface(instructionsSurface);
                    SDL_DestroyTexture(instructionsTexture);
                }
                break;
                
            case INSTRUCTIONS:
                window0.render(menuBackgroundTexture);
                
               
                {
                    string titleText = "HOW TO PLAY";
                    SDL_Surface* titleSurface = TTF_RenderText_Solid(titleFont, titleText.c_str(), textColor);
                    SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(window0.getRenderer(), titleSurface);
                    
                    SDL_Rect titleRect;
                    titleRect.x = (1000 - titleSurface->w) / 2;
                    titleRect.y = 50;
                    titleRect.w = titleSurface->w;
                    titleRect.h = titleSurface->h;
                    
                    SDL_RenderCopy(window0.getRenderer(), titleTexture, NULL, &titleRect);
                    SDL_FreeSurface(titleSurface);
                    SDL_DestroyTexture(titleTexture);
                }
                
                
                {
                    int yPos = 150;
                    
                    {
                        const char* line = "Game Instructions:";
                        SDL_Surface* lineSurface = TTF_RenderText_Solid(font, line, textColor);
                        SDL_Texture* lineTexture = SDL_CreateTextureFromSurface(window0.getRenderer(), lineSurface);
                        
                        SDL_Rect lineRect;
                        lineRect.x = 250;
                        lineRect.y = yPos;
                        lineRect.w = lineSurface->w;
                        lineRect.h = lineSurface->h;
                        
                        SDL_RenderCopy(window0.getRenderer(), lineTexture, NULL, &lineRect);
                        SDL_FreeSurface(lineSurface);
                        SDL_DestroyTexture(lineTexture);
                        yPos += 60;
                    }
                    
                   
                    {
                        const char* line = "- Use LEFT and RIGHT arrow keys to move the knight";
                        SDL_Surface* lineSurface = TTF_RenderText_Solid(font, line, textColor);
                        SDL_Texture* lineTexture = SDL_CreateTextureFromSurface(window0.getRenderer(), lineSurface);
                        
                        SDL_Rect lineRect;
                        lineRect.x = 250;
                        lineRect.y = yPos;
                        lineRect.w = lineSurface->w;
                        lineRect.h = lineSurface->h;
                        
                        SDL_RenderCopy(window0.getRenderer(), lineTexture, NULL, &lineRect);
                        SDL_FreeSurface(lineSurface);
                        SDL_DestroyTexture(lineTexture);
                        yPos += 40;
                    }
                    
                   
                    {
                        const char* line = "- Press SPACE to jump";
                        SDL_Surface* lineSurface = TTF_RenderText_Solid(font, line, textColor);
                        SDL_Texture* lineTexture = SDL_CreateTextureFromSurface(window0.getRenderer(), lineSurface);
                        
                        SDL_Rect lineRect;
                        lineRect.x = 250;
                        lineRect.y = yPos;
                        lineRect.w = lineSurface->w;
                        lineRect.h = lineSurface->h;
                        
                        SDL_RenderCopy(window0.getRenderer(), lineTexture, NULL, &lineRect);
                        SDL_FreeSurface(lineSurface);
                        SDL_DestroyTexture(lineTexture);
                        yPos += 40;
                    }
                    
                   
                    {
                        const char* line = "- Avoid the slimes and falling bombs";
                        SDL_Surface* lineSurface = TTF_RenderText_Solid(font, line, textColor);
                        SDL_Texture* lineTexture = SDL_CreateTextureFromSurface(window0.getRenderer(), lineSurface);
                        
                        SDL_Rect lineRect;
                        lineRect.x = 250;
                        lineRect.y = yPos;
                        lineRect.w = lineSurface->w;
                        lineRect.h = lineSurface->h;
                        
                        SDL_RenderCopy(window0.getRenderer(), lineTexture, NULL, &lineRect);
                        SDL_FreeSurface(lineSurface);
                        SDL_DestroyTexture(lineTexture);
                        yPos += 40;
                    }
                    
                    
                    {
                        const char* line = "- Survive as long as possible to increase your score";
                        SDL_Surface* lineSurface = TTF_RenderText_Solid(font, line, textColor);
                        SDL_Texture* lineTexture = SDL_CreateTextureFromSurface(window0.getRenderer(), lineSurface);
                        
                        SDL_Rect lineRect;
                        lineRect.x = 250;
                        lineRect.y = yPos;
                        lineRect.w = lineSurface->w;
                        lineRect.h = lineSurface->h;
                        
                        SDL_RenderCopy(window0.getRenderer(), lineTexture, NULL, &lineRect);
                        SDL_FreeSurface(lineSurface);
                        SDL_DestroyTexture(lineTexture);
                    }
                }
                
                
                {
                    SDL_SetRenderDrawColor(window0.getRenderer(), 50, 50, 50, 255);
                    SDL_RenderFillRect(window0.getRenderer(), &backButtonRect);
                    
                    SDL_SetRenderDrawColor(window0.getRenderer(), 255, 255, 255, 255);
                    SDL_RenderDrawRect(window0.getRenderer(), &backButtonRect);
                    
                    string backText = "BACK TO MENU";
                    SDL_Surface* backSurface = TTF_RenderText_Solid(font, backText.c_str(), 
                                                               backButtonHovered ? highlightColor : textColor);
                    SDL_Texture* backTexture = SDL_CreateTextureFromSurface(window0.getRenderer(), backSurface);
                    
                    SDL_Rect backTextRect;
                    backTextRect.x = backButtonRect.x + (backButtonRect.w - backSurface->w) / 2;
                    backTextRect.y = backButtonRect.y + (backButtonRect.h - backSurface->h) / 2;
                    backTextRect.w = backSurface->w;
                    backTextRect.h = backSurface->h;
                    
                    SDL_RenderCopy(window0.getRenderer(), backTexture, NULL, &backTextRect);
                    SDL_FreeSurface(backSurface);
                    SDL_DestroyTexture(backTexture);
                }
                break;
                
            case GAME:
                window0.render(backgroundTexture);
                window0.render(knight);
                
                for (Enemy& slime : enemies) {
                    slime.render(window0.getRenderer());
                }
                for (auto& flyer : flyers) {
                    flyer.render(window0.getRenderer());
                }
                for (auto& bomb : bombs) {
                    bomb.render(window0.getRenderer());
                }
               
                for (int i = 0; i < hearts; ++i) {
                    SDL_Rect heartDst = { 20 + i * 40, 20, 32, 32 };
                    SDL_RenderCopy(window0.getRenderer(), heartTexture, NULL, &heartDst);
                }
                
                
                {
                    std::string scoreText = "Score: " + std::to_string(score);
                    SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
                    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(window0.getRenderer(), textSurface);
                    
                    SDL_Rect textRect;
                    textRect.x = 800; 
                    textRect.y = 20;
                    textRect.w = textSurface->w;
                    textRect.h = textSurface->h;
                    
                    SDL_RenderCopy(window0.getRenderer(), textTexture, NULL, &textRect);
                    SDL_FreeSurface(textSurface);
                    SDL_DestroyTexture(textTexture);
                }
                break;
                
            case GAME_OVER:
                window0.render(backgroundTexture);
                
                
                {
                    std::string gameOverText = "GAME OVER";
                    SDL_Surface* gameOverSurface = TTF_RenderText_Solid(font, gameOverText.c_str(), textColor);
                    SDL_Texture* gameOverTexture = SDL_CreateTextureFromSurface(window0.getRenderer(), gameOverSurface);
                    
                    SDL_Rect gameOverRect;
                    gameOverRect.x = (1000 - gameOverSurface->w) / 2;
                    gameOverRect.y = 200;
                    gameOverRect.w = gameOverSurface->w;
                    gameOverRect.h = gameOverSurface->h;
                    
                    SDL_RenderCopy(window0.getRenderer(), gameOverTexture, NULL, &gameOverRect);
                    SDL_FreeSurface(gameOverSurface);
                    SDL_DestroyTexture(gameOverTexture);
                }
                
            
                {
                    std::string finalScoreText = "Final Score: " + std::to_string(score);
                    SDL_Surface* finalScoreSurface = TTF_RenderText_Solid(font, finalScoreText.c_str(), textColor);
                    SDL_Texture* finalScoreTexture = SDL_CreateTextureFromSurface(window0.getRenderer(), finalScoreSurface);
                    
                    SDL_Rect finalScoreRect;
                    finalScoreRect.x = (1000 - finalScoreSurface->w) / 2;
                    finalScoreRect.y = 250;
                    finalScoreRect.w = finalScoreSurface->w;
                    finalScoreRect.h = finalScoreSurface->h;
                    
                    SDL_RenderCopy(window0.getRenderer(), finalScoreTexture, NULL, &finalScoreRect);
                    SDL_FreeSurface(finalScoreSurface);
                    SDL_DestroyTexture(finalScoreTexture);
                }
                
             
                if (replayButtonTexture) {
                    SDL_RenderCopy(window0.getRenderer(), replayButtonTexture, NULL, &replayButtonRect);
                } else {
                    SDL_SetRenderDrawColor(window0.getRenderer(), 100, 100, 100, 255);
                    SDL_RenderFillRect(window0.getRenderer(), &replayButtonRect);
                    
                    SDL_SetRenderDrawColor(window0.getRenderer(), 255, 255, 255, 255);
                    SDL_RenderDrawRect(window0.getRenderer(), &replayButtonRect);
                    
                    string replayText = "REPLAY";
                    SDL_Surface* replaySurface = TTF_RenderText_Solid(font, replayText.c_str(), 
                                                                   replayButtonHovered ? highlightColor : textColor);
                    SDL_Texture* replayTexture = SDL_CreateTextureFromSurface(window0.getRenderer(), replaySurface);
                    
                    SDL_Rect replayTextRect;
                    replayTextRect.x = replayButtonRect.x + (replayButtonRect.w - replaySurface->w) / 2;
                    replayTextRect.y = replayButtonRect.y + (replayButtonRect.h - replaySurface->h) / 2;
                    replayTextRect.w = replaySurface->w;
                    replayTextRect.h = replaySurface->h;
                    
                    SDL_RenderCopy(window0.getRenderer(), replayTexture, NULL, &replayTextRect);
                    
                    SDL_FreeSurface(replaySurface);
                    SDL_DestroyTexture(replayTexture);
                }
                break;
        }
        
        window0.display();
    }
    
 
    if (hurtSound) {
        Mix_FreeChunk(hurtSound);
    }
    if (explosionSound) {
        Mix_FreeChunk(explosionSound);
    }
    if (gameOverMusic) {
        Mix_FreeMusic(gameOverMusic);
    }
    
    SDL_DestroyTexture(knightTexture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(slimeTexture);
    SDL_DestroyTexture(heartTexture);
    SDL_DestroyTexture(flyingEnemyTex);
    SDL_DestroyTexture(bombTex);
    SDL_DestroyTexture(menuBackgroundTexture);
    if (replayButtonTexture) {
        SDL_DestroyTexture(replayButtonTexture);
    }
    
    window0.cleanUp();
    TTF_CloseFont(font);
    TTF_CloseFont(titleFont);
    TTF_Quit();
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
    return 0;
}
