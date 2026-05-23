//
// Created by dimitrie on 23.05.2026.
//

#ifndef OOP_MENU_H
#define OOP_MENU_H

#include<iostream>
#include <SFML/Graphics.hpp>
#include "./VertexArrayUtility.h"
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

    void updateBackgroundAndCursor() {
        backgroundAndCursorVertexArray.clear();
        //background
        VertexArrayUtility::insertRectangle(backgroundAndCursorVertexArray, 0, 0,
                                            static_cast<int16_t>(window->getSize().x),
                                            static_cast<int16_t>(window->getSize().y), backgroundColor);
        //cursor
        VertexArrayUtility::insertRectangle(backgroundAndCursorVertexArray, 0,
                                            static_cast<int16_t>((cursor + 1) * lineHeight),
                                            static_cast<int16_t>(window->getSize().x), lineHeight, cursorColor);
    }

    void manageResizedEvent() {
        text.manageResizedEvent();
        updateBackgroundAndCursor();
    }

    int manageKey(const sf::Keyboard::Scancode k) {
        const int16_t oldCursor = cursor;
        if (k == sf::Keyboard::Scancode::Right || k == sf::Keyboard::Scancode::Down) {
            cursor++;
            if (cursor >= len) {
                cursor = 0;
            }
        } else if (k == sf::Keyboard::Scancode::Left || k == sf::Keyboard::Scancode::Up) {
            cursor--;
            if (cursor < 0) {
                cursor = static_cast<int16_t>(len - 1);
            }
        } else if (k == sf::Keyboard::Scancode::Enter) {
            return actions[cursor];
        }

        if (oldCursor != cursor) {
            updateBackgroundAndCursor();
        }

        return 0;
    }

    int manageOtherEvents() override {
        if (const std::optional<sf::Event> event = getEventForDerivedClasses(); event->getIf<sf::Event::Resized>()) {
            manageResizedEvent();
        } else if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            manageKey(keyPressed->scancode);
        }
        return 0;
    }

    int draw() override {
        window->draw(backgroundAndCursorVertexArray);
        text.draw();
        return 0;
    }

public:
    explicit Menu(sf::RenderWindow *window_, std::string parameters_, std::vector<int8_t> actions_,
                  const uint8_t pixelSize_, const sf::Color textColor_, const sf::Color backgroundColor_,
                  const sf::Color cursorColor_) : Scene(window_), backgroundColor(backgroundColor_),
                                                  cursorColor(cursorColor_),
                                                  actions(std::move(actions_)) {
        window = window_;

        lineHeight = static_cast<int16_t>(pixelSize_ * 9);

        backgroundAndCursorVertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);
        updateBackgroundAndCursor();

        len = 0;
        for (const char parameter: parameters_) {
            if (parameter == '\n') {
                len++;
            }
        }

        text = ReadOnlyText{
            window, std::move(parameters_), 0, 0, pixelSize_, window->getSize().x, window->getSize().y, textColor_,
            sf::Color::Transparent, false
        };
    }

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