//
// Created by dimitrie on 20.06.2026.
//

#ifndef OOP_VERTEXARRAYUTILITY_H
#define OOP_VERTEXARRAYUTILITY_H

#include <sstream>
#include <SFML/Graphics.hpp>

namespace VertexArrayUtility {
    void insertRectangle(sf::VertexArray &v, int16_t x, int16_t y, int16_t w, int16_t h, sf::Color color);

    void moveAt(sf::VertexArray &v, int16_t newX, int16_t newY);

    void insertChar(sf::VertexArray &v, char c, int16_t x, int16_t y, int16_t pixelSize, sf::Color color);
}

#endif //OOP_VERTEXARRAYUTILITY_H
