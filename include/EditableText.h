//
// Created by dimitrie on 23.05.2026.
//

#ifndef OOP_EDITABLETEXT_H
#define OOP_EDITABLETEXT_H

#include<iostream>
#include <SFML/Graphics.hpp>
#include "./Scene.h"


class EditableText : public Scene {
    friend std::ostream &operator<<(std::ostream &os, const EditableText &obj) {
        return os
               << "\nIf you really want to see the contents of this class, take a look at the following lines:"
               << "\nwindow: " << obj.window
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
               << "\nbg (the vertex count): " << obj.backgroundVertexArray.getVertexCount()
               << "\ntextVertexArray (the vertex count): " << obj.textVertexArray.getVertexCount()
               << "\ncursorL: " << obj.cursorL
               << "\ncursorR: " << obj.cursorR
               << "\nselectionHead: " << obj.selectionHead
               << "\nstep: " << obj.step << "\n";
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
    sf::Color cursorColor;

    sf::VertexArray backgroundVertexArray;
    sf::VertexArray textVertexArray;

    size_t cursorL = 0;
    size_t cursorR = 0;
    size_t *selectionHead = &cursorR;
    size_t step = 0;
    bool ctrl = false;
    bool shift = false;

    void updateVertexArray();

    void deleteSelectedText();

    int draw() override;

    int manageOtherEvents() override;

    void manageResizedEvent();

    void manageEnteredText(const sf::Event::TextEntered *enteredText);

    int manageKey(sf::Keyboard::Scancode k);

public:
    explicit EditableText(sf::RenderWindow *window_, std::string text_, const int16_t x_, const int16_t y_,
                          const uint8_t pixelSize_,
                          const size_t W_, const size_t H_, const sf::Color color_,
                          const sf::Color background_, const sf::Color cursorColor_) : Scene(window_), window(window_),
        text(std::move(text_)), x(x_),
        y(y_),
        pixelSize(pixelSize_), W(W_), H(H_),
        textColor(color_),
        backgroundColor(background_), cursorColor(cursorColor_) {
        updateVertexArray();
    }

    [[nodiscard]] Scene *clone() const override {
        return new EditableText(*this);
    }
};


#endif //OOP_EDITABLETEXT_H