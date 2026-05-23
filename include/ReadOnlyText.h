//
// Created by dimitrie on 23.05.2026.
//

#ifndef OOP_READONLYTEXT_H
#define OOP_READONLYTEXT_H

#include <iostream>
#include <SFML/Graphics.hpp>


class ReadOnlyText {
    friend std::ostream &operator<<(std::ostream &os, const ReadOnlyText &obj) {
        return os
               << "\nIf you really want to see the contents of this class, take a look at the following lines:"
               << "\ntext: " << obj.text
               << "\nx: " << obj.x
               << "\ny: " << obj.y
               << "\npixelSize: " << obj.pixelSize
               << "\nW: " << obj.W
               << "\nH: " << obj.H
               << "\ncolor: rgba(" << obj.textColor.r << ", " << obj.textColor.g << ", " << obj.textColor.b << ", " <<
               obj.textColor.a
               << ")"
               << "\nbackground: rgba(" << obj.backgroundColor.r << ", " << obj.backgroundColor.g << ", " << obj.
               backgroundColor.b <<
               ", " << obj.backgroundColor.a << ")"
               << "\nlineWrappable: " << obj.lineWrappable
               << "\nbg (the vertex count): " << obj.backgroundVertexArray.getVertexCount()
               << "\ntextVertexArray (the vertex count): " << obj.textVertexArray.getVertexCount()
               << "\n";
    }

    sf::RenderWindow *window;
    std::string text;
    int16_t x;
    int16_t y;
    int16_t pixelSize;
    size_t W;
    size_t H;
    sf::Color textColor;
    sf::Color backgroundColor;
    bool lineWrappable;

    sf::VertexArray backgroundVertexArray;
    sf::VertexArray textVertexArray;

    void updateVertexArray();

public:
    ReadOnlyText();

    explicit ReadOnlyText(sf::RenderWindow *window_, std::string text_, int16_t x_, int16_t y_,
    uint8_t pixelSize_,
                          size_t W_, size_t H_, sf::Color color_,
                          sf::Color background_, bool lineWrappable_ = true);

    ReadOnlyText(const ReadOnlyText &other) = default;

    ReadOnlyText(ReadOnlyText &&other) noexcept
        : window(other.window),
          text(std::move(other.text)),
          x(other.x),
          y(other.y),
          pixelSize(other.pixelSize),
          W(other.W),
          H(other.H),
          textColor(other.textColor),
          backgroundColor(other.backgroundColor),
          lineWrappable(other.lineWrappable),
          backgroundVertexArray(std::move(other.backgroundVertexArray)),
          textVertexArray(std::move(other.textVertexArray)) {
    }

    ReadOnlyText &operator=(const ReadOnlyText &other) {
        if (this == &other)
            return *this;
        window = other.window;
        text = other.text;
        x = other.x;
        y = other.y;
        pixelSize = other.pixelSize;
        W = other.W;
        H = other.H;
        textColor = other.textColor;
        backgroundColor = other.backgroundColor;
        lineWrappable = other.lineWrappable;
        backgroundVertexArray = other.backgroundVertexArray;
        textVertexArray = other.textVertexArray;
        return *this;
    }

    ReadOnlyText &operator=(ReadOnlyText &&other) noexcept {
        if (this == &other)
            return *this;
        window = other.window;
        text = std::move(other.text);
        x = other.x;
        y = other.y;
        pixelSize = other.pixelSize;
        W = other.W;
        H = other.H;
        textColor = other.textColor;
        backgroundColor = other.backgroundColor;
        lineWrappable = other.lineWrappable;
        backgroundVertexArray = std::move(other.backgroundVertexArray);
        textVertexArray = std::move(other.textVertexArray);
        return *this;
    }

    void draw() const;

    void manageResizedEvent();

    void moveAt(int16_t newX, int16_t newY);
};


#endif //OOP_READONLYTEXT_H