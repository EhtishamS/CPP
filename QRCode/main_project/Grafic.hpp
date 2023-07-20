#ifndef GRAFIC 
#define GRAFIC
#include <SFML/Graphics.hpp>
#include <iostream>

#define MODULE_SIZE 10

class Grafic{
    private:
        int **matrix;
        int size;
    public:
        Grafic(int **matrix, int size){
            this->matrix = matrix;
            this->size = size;

            sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "QR CODE", sf::Style::Titlebar | sf::Style::Close);
            window.setFramerateLimit(60);
            
            sf::RectangleShape rect[size][size];

            sf::Vector2u size_window = window.getSize();

            int offset_x = size_window.x/2 - (size*MODULE_SIZE)/2;
            int offset_y = size_window.y/2 - (size*MODULE_SIZE)/2;

            for(int i=0;i<size;i++){
                for(int j=0;j<size;j++){
                    if(matrix[i][j] == 3 || matrix[i][j] == 1){
                        rect[i][j].setPosition(sf::Vector2f(offset_x, offset_y+(i*MODULE_SIZE)));
                        rect[i][j].setSize(sf::Vector2f(MODULE_SIZE,MODULE_SIZE));
                        rect[i][j].setFillColor(sf::Color(0,0,0,255));
                    }

                    if(matrix[i][j] == 5){
                        rect[i][j].setPosition(sf::Vector2f(offset_x, offset_y+(i*MODULE_SIZE)));
                        rect[i][j].setSize(sf::Vector2f(MODULE_SIZE,MODULE_SIZE));
                        rect[i][j].setFillColor(sf::Color(0,0,255,255));
                    }

                    offset_x += MODULE_SIZE;
                }

                offset_x = size_window.x/2 - (size*MODULE_SIZE)/2;
            }

            while (window.isOpen()) {
                sf::Event event;
                while (window.pollEvent(event))
                {
                    if (event.type == sf::Event::Closed)
                        window.close();

                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                        window.close();
                }

                window.clear(sf::Color(255,255,255,255));
                for(int i=0;i<size;i++){
                    for(int j=0;j<size;j++){
                        window.draw(rect[i][j]);
                    }
                }
                window.display();
            }
        }
};

#endif