//
// Created by dimitrie on 23.05.2026.
//

#ifndef OOP_TILEPANEL_H
#define OOP_TILEPANEL_H

#include<iostream>
#include <SFML/Graphics.hpp>
#include "./VertexArrayUtility.h"

class TilePanel {
    int8_t cursorX = 0;
    int8_t cursorY = 0;
    sf::RenderWindow *window;
    const int16_t rows;
    const int16_t columns;
    const int16_t x;
    const int16_t y;
    const int16_t cellW;
    const int16_t cellH;
    const sf::Color tableColor;
    const sf::Color backgroundColor;
    const sf::Color cursorColor;
    sf::VertexArray backgroundVertexArray;
    sf::VertexArray tableVertexArray;
    sf::VertexArray cursorVertexArray;

public:
    // The actual sizes of w and h are slightly different from those provided at the function call
    TilePanel(sf::RenderWindow *window_, const int16_t rows_, const int16_t columns_, const int16_t x_,
              const int16_t y_, const int16_t w_,
              const int16_t h_,
              const sf::Color tableColor_, const sf::Color backgroundColor_,
              const sf::Color cursorColor_) : window(window_),
                                              rows(rows_),
                                              columns(columns_),
                                              x(x_),
                                              y(y_),
                                              cellW(static_cast<int16_t>(w_ / columns)),
                                              cellH(static_cast<int16_t>(h_ / rows)),
                                              tableColor(tableColor_),
                                              backgroundColor(backgroundColor_),
                                              cursorColor(cursorColor_) {
        backgroundVertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);
        VertexArrayUtility::insertRectangle(backgroundVertexArray, 0, 0, static_cast<int16_t>(window->getSize().x),
                                            static_cast<int16_t>(window->getSize().y), backgroundColor);

        tableVertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);

        // horizontal lines
        for (int i = 0; i <= rows; i++) {
            VertexArrayUtility::insertRectangle(tableVertexArray, x, static_cast<int16_t>(y + cellH * i),
                                                static_cast<int16_t>(cellW * columns + 10), 10, tableColor);
        }
        // vertical lines
        for (int i = 0; i <= columns; i++) {
            VertexArrayUtility::insertRectangle(tableVertexArray, static_cast<int16_t>(x + cellW * i), y, 10,
                                                static_cast<int16_t>(cellH * rows), tableColor);
        }

        std::vector<sf::Color> t;
        int cell = rows * columns;
        int colorUnit = 16777215 / (rows * columns);
        std::cout << colorUnit;

        for (int c = 0; (c < 16777216) && cell; c += colorUnit) {
            int r = c;
            int g = r % 256;
            r /= 256;
            int b = r % 256;
            r /= 256;
            std::cout << r << " " << g << " " << b << "\n";

            t.insert(t.end(), sf::Color(r, g, b));
            cell--;
        }
        int p = 0;
        for (int j = 0; j < rows; j++) {
            for (int i = 0; i < columns; i++) {
                VertexArrayUtility::insertRectangle(tableVertexArray, static_cast<int16_t>(x + 10 + cellW * i),
                                                    static_cast<int16_t>(y + 10 + cellH * j),
                                                    static_cast<int16_t>(cellW - 10), static_cast<int16_t>(cellH - 10),
                                                    t[p++]);
            }
        }

        // cursor
        cursorVertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);
        // horizontal lines
        for (int i = 0; i <= 1; i++) {
            VertexArrayUtility::insertRectangle(cursorVertexArray, x, static_cast<int16_t>(y + cellH * i),
                                                static_cast<int16_t>(cellW + 10), 10, cursorColor);
        }
        // vertical lines
        for (int i = 0; i <= 1; i++) {
            VertexArrayUtility::insertRectangle(cursorVertexArray, static_cast<int16_t>(x + cellW * i), y, 10, cellH,
                                                cursorColor);
        }
    };

    int play() {
        int returnedValue = -1;
        while (window->isOpen()) {
            while (const std::optional event = window->pollEvent()) {
                if (event->is<sf::Event::Closed>()) {
                    window->close();
                    std::cout << "Window closed\n";
                    return -2;
                }
                if (const auto *resized = event->getIf<sf::Event::Resized>()) {
                    std::cout << "New width: " << window->getSize().x << '\n'
                            << "New height: " << window->getSize().y << '\n';
                    // update the view to the new size of the window
                    sf::FloatRect visibleArea({0.f, 0.f}, sf::Vector2f(resized->size));
                    window->setView(sf::View(visibleArea));

                    manageResizedEvent();
                } else if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                        window->close();
                        std::cout << "Window closed\n";
                        return -2;
                    }
                    returnedValue = manageKey(keyPressed->scancode);
                }
            }
            if (returnedValue != -1) {
                break;
            }

            window->clear();

            draw();

            window->display();
        }
        return returnedValue;
    }

    void manageResizedEvent() {
        backgroundVertexArray.clear();
        VertexArrayUtility::insertRectangle(backgroundVertexArray, 0, 0, static_cast<int16_t>(window->getSize().x),
                                            static_cast<int16_t>(window->getSize().y), backgroundColor);
    }

    int manageKey(const sf::Keyboard::Scancode k) {
        const int8_t oldCursorX = cursorX;
        const int8_t oldCursorY = cursorY;
        if (k == sf::Keyboard::Scancode::Right) {
            cursorX++;
        } else if (k == sf::Keyboard::Scancode::Left) {
            cursorX--;
        }
        if (k == sf::Keyboard::Scancode::Up) {
            cursorY--;
        } else if (k == sf::Keyboard::Scancode::Down) {
            cursorY++;
        }
        //repair overflow/underflow movements
        if (cursorX == -1) {
            cursorX = 0;
        } else if (cursorX == columns) {
            cursorX--;
        }
        if (cursorY == -1) {
            cursorY = 0;
        } else if (cursorY == rows) {
            cursorY--;
        }
        //update cursor coordinates
        if ((cursorX != oldCursorX) || (cursorY != oldCursorY)) {
            VertexArrayUtility::moveAt(cursorVertexArray, static_cast<int16_t>(x + cellW * cursorX),
                                       static_cast<int16_t>(y + cellH * cursorY));
        }

        if (k == sf::Keyboard::Scancode::Enter) {
            return 0;
        }
        return -1;
    }

    void draw() const {
        window->draw(backgroundVertexArray);
        window->draw(tableVertexArray);
        window->draw(cursorVertexArray);
    }
};

#endif //OOP_TILEPANEL_H