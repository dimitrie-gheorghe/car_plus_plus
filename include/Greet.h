//
// Created by dimitrie on 23.05.2026.
//

#ifndef OOP_GREET_H
#define OOP_GREET_H

#include <SFML/Graphics.hpp>
#include "./Scene.h"
#include "./ReadOnlyText.h"

class Greet : public Scene {
    friend std::ostream &operator<<(std::ostream &os, const Greet &obj) {
        return os
               << "\nIf you really want to see the contents of this class, take a look at the following lines:"
               << "\nformerPos(x, y): " << obj.formerPos.x << ", " << obj.formerPos.y
               << "\nformerWindowW: " << obj.formerWindowW
               << "\nformerWindowH: " << obj.formerWindowH
               << "\nwindow: " << obj.window
               << "\nW: " << obj.W
               << "\nH: " << obj.H
               << "\npixelWidth: " << obj.pixelWidth
               << "\ntextSize: " << obj.textSize
               << "\nmatrix: " << obj.matrix
               << "\ngreeting: " << obj.greeting
               << "\nx: " << obj.x;
    }

    sf::Vector2i formerPos;
    unsigned int formerWindowW = 0;
    unsigned int formerWindowH = 0;
    sf::RenderWindow *window;
    size_t W;
    size_t H;
    const uint8_t pixelWidth = 2;
    size_t textSize;

    ReadOnlyText matrix;
    ReadOnlyText greeting;

    int16_t x;

    void setProperties();

    void resetProperties();

    int manageOtherEvents() override;

    int draw() override;

public:
    explicit Greet(sf::RenderWindow *window_, const std::string &s);

    [[nodiscard]] Scene *clone() const override {
        return new Greet(*this);
    }
};


#endif //OOP_GREET_H