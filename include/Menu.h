//
// Created by dimitrie on 23.05.2026.
//

#ifndef OOP_MENU_H
#define OOP_MENU_H

#include<iostream>
#include <SFML/Graphics.hpp>
#include "./Scene.h"
#include "./ReadOnlyText.h"

class Menu : public Scene {
    friend std::ostream &operator<<(std::ostream &os, const Menu &obj) {
        return os
               << "If you really want to see the contents of this class, take a look at the following lines:"
               << "window: " << obj.window
               << " text: " << obj.text
               << "\ncolor: rgba(" << obj.backgroundColor.r << ", " << obj.backgroundColor.g << ", " << obj.
               backgroundColor.b << ", " << obj.backgroundColor.a << ")"
               << "\ncursorColor: rgba(" << obj.cursorColor.r << ", " << obj.cursorColor.g << ", " << obj.
               cursorColor.b << ", " << obj.cursorColor.a << ")"
               << " actions size: " << obj.actions.size()
               << " cursor: " << obj.cursor
               << " backgroundAndCursorVertexArray (vertexCount): " << obj.backgroundAndCursorVertexArray.
               getVertexCount()
               << " len: " << obj.len
               << " lineHeight: " << obj.lineHeight;
    }

    sf::RenderWindow *window;
    ReadOnlyText text;
    const sf::Color backgroundColor;
    const sf::Color cursorColor;
    std::vector<int8_t> actions;
    int16_t cursor = 0;
    sf::VertexArray backgroundAndCursorVertexArray;
    int16_t len;
    int16_t lineHeight;

    void updateBackgroundAndCursor();

    void manageResizedEvent();

    int manageKey(sf::Keyboard::Scancode k);

    int manageOtherEvents() override;

    int draw() override;

public:
    explicit Menu(sf::RenderWindow *window_, std::string parameters_, std::vector<int8_t> actions_,
    uint8_t pixelSize_, sf::Color textColor_, sf::Color backgroundColor_,
    sf::Color cursorColor_);

    [[nodiscard]] Scene *clone() const override {
        return new Menu(*this);
    }

    // int play() {
    //     int returnedValue = -1;
    //     while (window->isOpen()) {
    //         while (const std::optional event = window->pollEvent()) {
    //             if (event->is<sf::Event::Closed>()) {
    //                 window->close();
    //                 std::cout << "Window closed\n";
    //                 return -2;
    //             }
    //             if (const auto *resized = event->getIf<sf::Event::Resized>()) {
    //                 std::cout << "New width: " << window->getSize().x << '\n'
    //                         << "New height: " << window->getSize().y << '\n';
    //                 // update the view to the new size of the window
    //                 sf::FloatRect visibleArea({0.f, 0.f}, sf::Vector2f(resized->size));
    //                 window->setView(sf::View(visibleArea));
    //
    //                 manageResizedEvent();
    //             } else if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
    //                 if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
    //                     window->close();
    //                     std::cout << "Window closed\n";
    //                     return -2;
    //                 }
    //                 returnedValue = manageKey(keyPressed->scancode);
    //             }
    //         }
    //         if (returnedValue != -1) {
    //             break;
    //         }
    //
    //         window->clear();
    //
    //         draw();
    //
    //         window->display();
    //     }
    //     return returnedValue;
    // }
};


#endif //OOP_MENU_H