//
// Created by dimitrie on 23.05.2026.
//

#ifndef OOP_TILEPANEL_H
#define OOP_TILEPANEL_H

#include <SFML/Graphics.hpp>

class TilePanel {
    int8_t cursorX = 0;
    int8_t cursorY = 0;
    sf::RenderWindow *window;
    const int16_t rows;
    const int16_t columns;
    const int16_t x;
    const int16_t y;
    const int16_t cellW;
    const int16_t cellH;
    const sf::Color tableColor;
    const sf::Color backgroundColor;
    const sf::Color cursorColor;
    sf::VertexArray backgroundVertexArray;
    sf::VertexArray tableVertexArray;
    sf::VertexArray cursorVertexArray;

public:
    // The actual sizes of w and h are slightly different from those provided at the function call
    TilePanel(sf::RenderWindow *window_, int16_t rows_, int16_t columns_, int16_t x_,
    int16_t y_, int16_t w_,
    int16_t h_,
    sf::Color tableColor_, sf::Color backgroundColor_,
    sf::Color cursorColor_);

    int play();

    void manageResizedEvent();

    int manageKey(sf::Keyboard::Scancode k);

    void draw() const;
};

#endif //OOP_TILEPANEL_H