#pragma once

#include <sdl/SDL.h>
#include <stdexcept>
#include <iostream>
#include <time.h>
#include <vector>

#include "headers/Math.hpp"

enum class directions{UP, DOWN, LEFT, RIGHT};

#define PLAYER 1
#define FRUIT 2

class Snake{
    private:
        int _tile_size_;
        int _platform_width_;
        int _platform_height_;

        Vector2f _player_head_pos_;
        Vector2f _food_pos_;

        std::vector<Vector2f> player_pos;

        int _move_x_;
        int _move_y_;
        int _pace_;
        int _score_;

        directions _direction_;
    public:
        Snake(int platformWidth, int platformHeight ,int tileSize, int snake_pace)
        :_tile_size_(tileSize), _platform_width_(platformWidth), _platform_height_(platformHeight){
            if(!(_platform_width_ % _tile_size_ == 0) || !(_platform_height_ % _tile_size_ == 0)){
                throw std::invalid_argument("Tile size is not divisble by platform width or platform height!");
            }

            if(snake_pace <= 0){
                throw std::invalid_argument("Snake pace cannot be zero or below!");
            }

            srand(time(NULL));

            _food_pos_.x = rand() % (_platform_width_/_tile_size_);
            _food_pos_.y = rand() % (_platform_height_/_tile_size_);

            _player_head_pos_.x = rand() % (_platform_width_/_tile_size_);
            _player_head_pos_.y = 0;

            player_pos.push_back(_player_head_pos_);

            _direction_ = directions::DOWN;

            _pace_ = snake_pace;
            _score_ = 0;
        };  

        Vector2f getFoodPos(){
            return _food_pos_;
        }

        Vector2f getPlayerHeadPos(){
            return _player_head_pos_;
        }

        int getScore(){
            return _score_;
        }

        void setDirection(directions& direction){
            if(_direction_ == directions::UP && direction != directions::DOWN){
                _direction_ = direction;
            } else if(_direction_ == directions::DOWN && direction != directions::UP){
                _direction_ = direction;
            } else if(_direction_ == directions::LEFT && direction != directions::RIGHT){
                _direction_ = direction;
            } else if(_direction_ == directions::RIGHT && direction != directions::LEFT){
                _direction_ = direction;
            }
        }

        std::vector<Vector2f> getPlayerPos(){
            for(int i=(_score_);i>0;i--){
                player_pos[i] = player_pos[i-1];
            }

            player_pos[0] = _player_head_pos_;

            return player_pos;
        }

        void addPace(){
            if(_direction_ == directions::UP){
                _player_head_pos_.y -= _pace_;
            } else if(_direction_ == directions::DOWN){
                _player_head_pos_.y += _pace_;
            } else if(_direction_ == directions::LEFT){
                _player_head_pos_.x -= _pace_;
            } else if(_direction_ == directions::RIGHT){
                _player_head_pos_.x += _pace_;
            }
        }

        void isFoodEatean(){
            if(_player_head_pos_.x == _food_pos_.x  && _player_head_pos_.y == _food_pos_.y){
                _food_pos_.x = rand() % (_platform_width_/_tile_size_);
                _food_pos_.y = rand() % (_platform_height_/_tile_size_);
                _score_++;

                player_pos.push_back(Vector2f(0,0));
            }
        }

        bool isSnakeOutOfBorder(){
            if((_player_head_pos_.x > (_platform_width_/_tile_size_)-1) || (_player_head_pos_.y > (_platform_height_/_tile_size_)-1)){
                return true;
            }

            if((_player_head_pos_.x < 0) || (_player_head_pos_.y < 0)){
                return true;
            }

            return false;
        }

        bool isSnakeEatingTale(){
            for(int i=_score_;i>0;i--){
                if(player_pos[i].x == _player_head_pos_.x && player_pos[i].y == _player_head_pos_.y){
                    return true;
                }
            }

            return false;
        }
};