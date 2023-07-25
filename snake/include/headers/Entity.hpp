#pragma once 
#include <sdl/SDL.h>
#include <sdl/SDL_image.h>
#include <headers/Math.hpp>

class Entity{
    private:
        Vector2f pos;
        SDL_Rect currentFrame;
        SDL_Texture* texture;
    public:
        Entity(Vector2f pos, SDL_Texture* texture);
        Vector2f& getPos();
        SDL_Texture* getTexture();
        SDL_Rect getCurrentFrame();
};

Entity::Entity(Vector2f pos, SDL_Texture* texture){
    this-> pos = pos;
    this->texture = texture; 

    currentFrame.x = 0;
    currentFrame.y = 0;
    currentFrame.w = 36;
    currentFrame.h = 36;
}

Vector2f& Entity::getPos(){
    return pos;
}

SDL_Texture* Entity::getTexture(){
    return texture;
}

SDL_Rect Entity::getCurrentFrame(){
    return currentFrame;
}
