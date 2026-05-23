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

    void setProperties() {
        formerWindowW = window->getSize().x;
        formerWindowH = window->getSize().y;
        formerPos = window->getPosition();

        sf::Vector2i desiredPos;
        desiredPos.x = static_cast<int>(formerPos.x + (window->getSize().x - W) / 2);
        desiredPos.y = static_cast<int>(formerPos.y + (window->getSize().y - H) / 2);

        window->create(sf::VideoMode({static_cast<unsigned int>(W), static_cast<unsigned int>(H)}),
                       "", sf::Style::None, sf::State::Windowed);
        window->setPosition(desiredPos);
        window->setFramerateLimit(20);
    }

    void resetProperties() {
        window->create(sf::VideoMode({formerWindowW, formerWindowH}), "car_plus_plus", sf::Style::Default,
                       sf::State::Windowed);
        window->setFramerateLimit(10);
    }

    int manageOtherEvents() override {
        return 0;
    };

    int draw() override {
        matrix.draw();
        greeting.draw();

        x = static_cast<int16_t>(x - 9 * pixelWidth);
        if (x < -static_cast<int16_t>(9 * 8 * pixelWidth * textSize)) {
            resetProperties();
            return -1;
        }
        greeting.moveAt(x, 0);
        return 0;
    }

public:
    explicit Greet(sf::RenderWindow *window_, const std::string &s) : Scene(window_) {
        textSize = s.size();

        W = pixelWidth * 9 * 8 * 3;
        H = pixelWidth * 9 * 8;

        window = window_;

        setProperties();

        x = static_cast<int16_t>(W);

        std::string backgroundText;
        constexpr uint8_t backgroundTextLen = 8 * 8 * 3;
        uint8_t i = 0;
        while (i < backgroundTextLen) {
            for (char c = ' '; i < backgroundTextLen && c <= '~'; c++) {
                backgroundText += c;
                i++;
            }
        }

        matrix = ReadOnlyText{window, backgroundText, 0, 0, pixelWidth, W, H, sf::Color::Black, sf::Color::White};
        greeting = ReadOnlyText{
            window,
            s, static_cast<int16_t>(W), 0, static_cast<uint8_t>(pixelWidth * 9),
            9 * 9 * pixelWidth * textSize, static_cast<size_t>(9 * 8 * pixelWidth), sf::Color::Black,
            sf::Color::Transparent
        };
    }

    [[nodiscard]] Scene *clone() const override {
        return new Greet(*this);
    }
};


#endif //OOP_GREET_H