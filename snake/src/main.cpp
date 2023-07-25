#include <sdl/SDL.h>
#include <sdl/SDL_image.h>
#include <vector>

#include "headers/RenderWindow.hpp"
#include "headers/Entity.hpp"
#include "headers/Math.hpp"
#include "headers/Utilis.hpp"
#include "headers/Snake.hpp"

int main(int argc, char* argv[]){
    const int width_window = 720, height_window = 720;

    if(SDL_Init(SDL_INIT_VIDEO) < 0 ) // 0 on sucess -1 on error
        throw (std::string)("SDL_Init... HAS FAILED. SDL_ERROR: ") + (std::string) SDL_GetError() + "\n";

    if (!(IMG_Init(IMG_INIT_PNG))) // returns all the activated flags 0 on failures
        throw (std::string)("IMG_Init... HAS FAILED. SDL_ERROR: ") + (std::string) SDL_GetError() + "\n";

    RenderWindow window("Game V1.0", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width_window, height_window, SDL_WINDOW_SHOWN); 
    
    bool isGameRunning = true;
    SDL_Event event;
    const float timeStep = 0.01f; // so it will update 100 times a second because 1/100
    float accumulator = 0.0f; 
    float currentTime = utils::hireTimeInSeconds();

    // Game variables
    int tile_size = 20;
    Snake snake(width_window, height_window, tile_size, 1);
    SDL_Texture* bodyTex = NULL;
    SDL_Texture* appleTex = NULL; 
    directions direction = directions::DOWN;

    appleTex = window.loadTexture("res/gfx/apple.png");
    bodyTex = window.loadTexture("res/gfx/snake.png");

    std::vector<Entity> sn;

    while(isGameRunning){
        int startTick = SDL_GetTicks64();
        float newTime = utils::hireTimeInSeconds();
        float frameTime = newTime - currentTime;

        currentTime = newTime;        
        accumulator += frameTime;

        while (accumulator >= timeStep){
            while(SDL_PollEvent(&event)){
                switch(event.type){
                    case SDL_QUIT:
                        isGameRunning = false;
                        break;
                }

                const Uint8* state = SDL_GetKeyboardState(NULL);

                if(state[SDL_SCANCODE_UP]){
                    direction = directions::UP;
                } else if(state[SDL_SCANCODE_DOWN]){
                    direction = directions::DOWN;
                } else if(state[SDL_SCANCODE_LEFT]){
                    direction = directions::LEFT;
                } else if(state[SDL_SCANCODE_RIGHT]){
                    direction = directions::RIGHT;
                }

                snake.setDirection(direction);
            }

            accumulator -= timeStep;
        }

        const float alpha = accumulator/timeStep;

        window.clear();

        {
            std::vector<Vector2f> pos = snake.getPlayerPos();

            for(Vector2f p : pos){
                sn.push_back(Entity(p, bodyTex));
            }

            snake.addPace();
            snake.isFoodEatean();
        }

        Entity food = Entity(snake.getFoodPos(), appleTex);
        
        window.render(food);

        for(Entity e : sn){
            window.render(e);
        }

        window.display();

        sn.clear();

        int frameTicks = SDL_GetTicks() - startTick;

        if (frameTicks < window.getRefreshRate()){
            SDL_Delay(window.getRefreshRate() - frameTicks);
        }

        SDL_Delay(10); // for making game slow or fast

        if(snake.isSnakeOutOfBorder() || snake.isSnakeEatingTale()){
            isGameRunning = false;
        }
    }
    
    window.cleanUp();
    SDL_Quit();

    std::cout << "Final score: " << snake.getScore() << std::endl;

    return 0;
}