//
// Created by dimitrie on 23.05.2026.
//

#include "../include/ReadOnlyText.h"
#include <SFML/Graphics.hpp>

#include "../include/VertexArrayUtility.h"

void ReadOnlyText::updateVertexArray() {
    textVertexArray.clear();
    textVertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);

    backgroundVertexArray.clear();
    backgroundVertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);
    VertexArrayUtility::insertRectangle(backgroundVertexArray, x, y, static_cast<int16_t>(W),
                                        static_cast<int16_t>(H), backgroundColor);

    if (lineWrappable) {
        std::string formattedText = text;
        const auto charPoz = static_cast<int16_t>(9 * pixelSize);
        const auto rightLimit = static_cast<int16_t>(W + x - charPoz);
        const size_t rowLen = (W + x) / charPoz;

        int16_t i = 0;
        while (i < static_cast<int16_t>(formattedText.size())) {
            if (formattedText[i] == '\t') {
                formattedText[i] = ' ';
                formattedText.insert(i, "   ");
                i += 4;
            } else if (formattedText[i] == '\n') {
                formattedText[i] = ' ';
                i++;
                while (i % rowLen) {
                    formattedText.insert(i, " ");
                    i++;
                }
            } else {
                i++;
            }
        }

        i = 0;
        const auto size = static_cast<int16_t>(formattedText.size());
        for (int16_t r = y; i < size; r = static_cast<int16_t>(r + charPoz)) {
            for (int16_t c = x; c <= rightLimit && i < size; c = static_cast<int16_t>(c + charPoz)) {
                VertexArrayUtility::insertChar(textVertexArray, formattedText[i], c, r, pixelSize, textColor);
                i++;
            }
        }
    } else {
        std::string formattedText = text;
        const auto charPoz = static_cast<int16_t>(9 * pixelSize);

        int16_t i = 0;
        while (i < static_cast<int16_t>(formattedText.size())) {
            if (formattedText[i] == '\t') {
                formattedText[i] = ' ';
                formattedText.insert(i, "   ");
                i += 4;
            } else {
                i++;
            }
        }

        i = 0;
        const auto size = static_cast<int16_t>(formattedText.size());
        int16_t r = y;
        while (i < size) {
            int16_t c = x;
            while (i < size && formattedText[i] != '\n') {
                VertexArrayUtility::insertChar(textVertexArray, formattedText[i], c, r, pixelSize, textColor);
                i++;
                c = static_cast<int16_t>(c + charPoz);
            }
            if (formattedText[i] == '\n') {
                i++;
            }
            r = static_cast<int16_t>(r + charPoz);
        }
    }
}

ReadOnlyText::ReadOnlyText() : lineWrappable(true) {
    window = nullptr;
    x = 0;
    y = 0;
    pixelSize = 5;
    W = 0;
    H = 0;
}

ReadOnlyText::ReadOnlyText(sf::RenderWindow *window_, std::string text_, const int16_t x_, const int16_t y_,
                                    const uint8_t pixelSize_,
                                    const size_t W_, const size_t H_, const sf::Color color_,
                                    const sf::Color background_,
                                    const bool lineWrappable_) : text(std::move(text_)),
                                                                        x(x_), y(y_),
                                                                        pixelSize(pixelSize_), W(W_), H(H_),
                                                                        textColor(color_),
                                                                        backgroundColor(background_),
                                                                        lineWrappable(lineWrappable_) {
    window = window_;
    updateVertexArray();
}

void ReadOnlyText::draw() const {
    window->draw(backgroundVertexArray);
    window->draw(textVertexArray);
}

void ReadOnlyText::manageResizedEvent() {
    //reflow the text
    if (lineWrappable) {
        W = window->getSize().x;
        H = window->getSize().y;
        updateVertexArray();
    }
}

void ReadOnlyText::moveAt(const int16_t newX, const int16_t newY) {
    const auto stepX = static_cast<int16_t>(newX - x);
    const auto stepY = static_cast<int16_t>(newY - y);
    const size_t count = textVertexArray.getVertexCount();
    for (size_t i = 0; i < count; i++) {
        textVertexArray[i].position.x += static_cast<float>(stepX);
        textVertexArray[i].position.x += static_cast<float>(stepY);
    }
    x = newX;
    y = newY;
}
