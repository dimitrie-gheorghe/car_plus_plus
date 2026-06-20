//
// Created by dimitrie on 20.06.2026.
//

#include "../include/ReadOnlyText.h"
#include <utility>
#include <fstream>
#include <ranges>
#include <string>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include "../ext/portable-file-dialogs.h"
#include "../include/VertexArrayUtility.h"

void ReadOnlyText::updateVertexArray() {
    textVertexArray.clear();
    textVertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);

    std::string formattedText = text;
    const auto charPoz = static_cast<int16_t>(9 * pixelSize);
    const auto rightLimit = static_cast<int16_t>(W + x - charPoz);
    const size_t rowLen = (W + x) / charPoz;

    size_t i = 0;
    if (lineWrappable) {
        while (i < formattedText.size()) {
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
            } else { i++; }
        }
        i = 0;
        const size_t size = formattedText.size();
        for (int16_t r = y; i < size; r = static_cast<int16_t>(r + charPoz)) {
            for (int16_t c = x; c <= rightLimit && i < size; c = static_cast<int16_t>(c + charPoz)) {
                VertexArrayUtility::insertChar(textVertexArray, formattedText[i], c, r, pixelSize, textColor);
                i++;
            }
        }
    } else {
        while (i < formattedText.size()) {
            if (formattedText[i] == '\t') {
                formattedText[i] = ' ';
                formattedText.insert(i, "   ");
                i += 4;
            } else {
                i++;
            }
        }
        i = 0;
        const size_t size = formattedText.size();
        int16_t r = y;
        while (i < size) {
            int16_t c = x;
            while (i < size && formattedText[i] != '\n') {
                VertexArrayUtility::insertChar(textVertexArray, formattedText[i], c, r, pixelSize, textColor);
                i++;
                c = static_cast<int16_t>(c + charPoz);
            }
            if (i < size && formattedText[i] == '\n') {
                i++;
            }
            r = static_cast<int16_t>(r + charPoz);
        }
    }
}

ReadOnlyText::ReadOnlyText(sf::RenderTarget *target_, std::string text_, const int16_t x_, const int16_t y_,
                           const uint8_t pixelSize_, const size_t W_, const size_t H_, const sf::Color color_,
                           const bool lineWrappable_)
    : target(target_), text(std::move(text_)), x(x_), y(y_), pixelSize(pixelSize_), W(W_), H(H_),
      textColor(color_), lineWrappable(lineWrappable_) {
    updateVertexArray();
}

void ReadOnlyText::setTextColor(const sf::Color &color) {
    textColor = color;
}

void ReadOnlyText::setPixelSize(const uint8_t &pixel_size) {
    pixelSize = pixel_size;
}

void ReadOnlyText::draw() const {
    target->draw(textVertexArray);
}

void ReadOnlyText::refresh() {
    updateVertexArray();
}

void ReadOnlyText::manageResizedEvent() {
    if (lineWrappable) {
        W = target->getSize().x;
        H = target->getSize().y;
        updateVertexArray();
    }
}

void ReadOnlyText::moveAt(const int16_t newX, const int16_t newY) {
    const auto stepX = static_cast<int16_t>(newX - x);
    const auto stepY = static_cast<int16_t>(newY - y);
    const size_t count = textVertexArray.getVertexCount();
    for (size_t i = 0; i < count; i++) {
        textVertexArray[i].position.x += static_cast<float>(stepX);
        textVertexArray[i].position.y += static_cast<float>(stepY);
    }
    x = newX;
    y = newY;
}

std::ostream &operator<<(std::ostream &os, const ReadOnlyText &obj) {
    os << "ReadOnlyText component -> text content: \"" << obj.text << "\" pos: (" << obj.x << "," << obj.y << ")";
    return os;
}

ReadOnlyText::ReadOnlyText() : target(nullptr), x(0), y(0), pixelSize(5), W(0), H(0), lineWrappable(true) {
}
