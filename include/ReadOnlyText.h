//
// Created by dimitrie on 20.06.2026.
//

#ifndef OOP_READONLYTEXT_H
#define OOP_READONLYTEXT_H

#include <filesystem>
#include <string>
#include <SFML/Graphics.hpp>

class ReadOnlyText {
    sf::RenderTarget *target;
    std::string text;
    int16_t x;
    int16_t y;
    int16_t pixelSize;
    size_t W;
    size_t H;
    sf::Color textColor;
    bool lineWrappable;
    sf::VertexArray textVertexArray;

    void updateVertexArray();

public:
    friend std::ostream &operator<<(std::ostream &os, const ReadOnlyText &obj);

    ReadOnlyText();

    explicit ReadOnlyText(sf::RenderTarget *target_, std::string text_, int16_t x_, int16_t y_,
                          uint8_t pixelSize_, size_t W_, size_t H_, sf::Color color_,
                          bool lineWrappable_ = true);

    void setTextColor(const sf::Color &color);

    void setPixelSize(const uint8_t &pixel_size);

    void draw() const;

    void refresh();

    void manageResizedEvent();

    void moveAt(const int16_t newX, const int16_t newY);
};


#endif //OOP_READONLYTEXT_H
