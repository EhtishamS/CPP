#pragma once

struct Vector2f{
    Vector2f():x(0.0), y(0.0){};
    Vector2f(int pos_x, int pos_y):x(pos_x), y(pos_y){};
    int x, y;
};