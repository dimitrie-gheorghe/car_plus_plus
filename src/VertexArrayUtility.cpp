//
// Created by dimitrie on 20.06.2026.
//

#include "../include/VertexArrayUtility.h"
#include <fstream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "../ext/portable-file-dialogs.h"
#include "../include/FontManager.h"

void VertexArrayUtility::insertRectangle(sf::VertexArray &v, const int16_t x, const int16_t y, const int16_t w,
                                         const int16_t h, const sf::Color color) {
    v.append({{static_cast<float>(x), static_cast<float>(y)}, color, {0.0f, 0.0f}});
    v.append({{static_cast<float>(x + w), static_cast<float>(y)}, color, {0.0f, 0.0f}});
    v.append({{static_cast<float>(x), static_cast<float>(y + h)}, color, {0.0f, 0.0f}});
    v.append({{static_cast<float>(x + w), static_cast<float>(y)}, color, {0.0f, 0.0f}});
    v.append({{static_cast<float>(x), static_cast<float>(y + h)}, color, {0.0f, 0.0f}});
    v.append({{static_cast<float>(x + w), static_cast<float>(y + h)}, color, {0.0f, 0.0f}});
}

void VertexArrayUtility::moveAt(sf::VertexArray &v, const int16_t newX, const int16_t newY) {
    if (const size_t len = v.getVertexCount()) {
        const float stepX = static_cast<float>(newX) - v[0].position.x;
        const float stepY = static_cast<float>(newY) - v[0].position.y;
        for (size_t i = 0; i < len; i++) {
            v[i].position.x += stepX;
            v[i].position.y += stepY;
        }
    }
}

void VertexArrayUtility::insertChar(sf::VertexArray &v, const char c, const int16_t x, const int16_t y,
                                    const int16_t pixelSize,
                                    const sf::Color color) {
    v.setPrimitiveType(sf::PrimitiveType::Triangles);

    if (c != ' ') {
        uint16_t offset;
        if (c < '!' || c > '~') {
            offset = (95 - 1) * 8;
        } else {
            offset = (c - '!') * 8;
        }
        for (uint16_t i = 0; i < 8; i++) {
            const FontManager &fontManager = FontManager::getInstance();
            uint8_t line = fontManager.getByte(offset + i);
            //uint8_t line = FontManager::getInstance().getByte(offset + i);
            for (int16_t j = 0; j < 8; j++) {
                const uint8_t _ = line & 1;
                line >>= 1;
                if (_) {
                    insertRectangle(v, static_cast<int16_t>(x + pixelSize * (7 - j)),
                                    static_cast<int16_t>(y + pixelSize * i), pixelSize,
                                    pixelSize, color);
                }
            }
        }
    }
}
