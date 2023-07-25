#pragma once

#include <sdl/SDL.h>

namespace utils{
    inline float hireTimeInSeconds(){
        float t = SDL_GetTicks64();
        t *= 0.001;

        return t;
    }
}