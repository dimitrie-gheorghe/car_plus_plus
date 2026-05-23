//
// Created by dimitrie on 23.05.2026.
//

#ifndef OOP_VERTEXARRAYUTILITY_H
#define OOP_VERTEXARRAYUTILITY_H

#include <SFML/Graphics.hpp>

class VertexArrayUtility {
    static const uint8_t ascii[8 * 95];

public:
    static void insertRectangle(sf::VertexArray &v, int16_t x, int16_t y, int16_t w,
                                int16_t h, sf::Color color);

    static void moveAt(sf::VertexArray &v, int16_t newX, int16_t newY);

    static void translateX(sf::VertexArray &v, float step);

    static void translateY(sf::VertexArray &v, float step);

    static void insertChar(sf::VertexArray &v, char c, int16_t x, int16_t y, int16_t pixelSize,
                           sf::Color color);
};


#endif //OOP_VERTEXARRAYUTILITY_H
