#pragma once

#include <sdl/SDL.h>
#include <sdl/SDL_image.h>
#include <Entity.hpp>
#include <exception>
#include <string>

class RenderWindow {
    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
    public:
        RenderWindow(const char* title, int x, int y, int w, int h, Uint32 flags);
        SDL_Texture* loadTexture(const char* filePath);
        int getRefreshRate();
        void cleanUp();
        void clear();
        void render(Entity& entity);
        void display();
        
        //~RenderWindow();
};

RenderWindow::RenderWindow(const char* title, int x, int y, int w, int h, Uint32 flags)
:window(NULL), renderer(NULL) {
    window = SDL_CreateWindow(title, x, y, w, h, flags);

    if(window == NULL){
        throw (std::string)("Window failed to init. Error: ") + (std::string) (SDL_GetError()) + "\n"; 
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED && SDL_RENDERER_PRESENTVSYNC); 

    if(renderer == NULL){
        throw (std::string)("Renderer failed to init. Error: ") + (std::string) (SDL_GetError()) + "\n";
    }
}

SDL_Texture* RenderWindow::loadTexture(const char* filePath){
    SDL_Texture* texture = NULL;
    texture = IMG_LoadTexture(renderer, filePath);
    
    if (texture == NULL){ 
        throw (std::string)("Failed to load a texture. Error: ") + (std::string)(SDL_GetError()) + "\n";
    }

    return texture; 
}

int RenderWindow::getRefreshRate(){
    int displayIndex = SDL_GetWindowDisplayIndex(window);
    SDL_DisplayMode mode;
    SDL_GetDisplayMode(displayIndex, 0, &mode); // 0 gives highest mode

    return mode.refresh_rate;

}

void RenderWindow::cleanUp(){
    SDL_DestroyWindow(window);
}

void RenderWindow::clear(){
    SDL_RenderClear(renderer);
}

void RenderWindow::render(Entity& entity){
    SDL_Rect src, dst;
    src.x = entity.getCurrentFrame().x; 
    src.y = entity.getCurrentFrame().y; 
    src.w = entity.getCurrentFrame().w; 
    src.h = entity.getCurrentFrame().h;
    
    dst.x = entity.getPos().x * 20; 
    dst.y = entity.getPos().y * 20; 
    dst.w = 20; 
    dst.h = 20;

    SDL_RenderCopy(renderer , entity.getTexture(), &src, &dst);
}

void RenderWindow::display(){
    SDL_RenderPresent(renderer);    
}

