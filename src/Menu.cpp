//
// Created by dimitrie on 23.05.2026.
//

#include "../include/Menu.h"
#include "../include/VertexArrayUtility.h"

void Menu::updateBackgroundAndCursor() {
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

void Menu::manageResizedEvent() {
    text.manageResizedEvent();
    updateBackgroundAndCursor();
}

int Menu::manageKey(const sf::Keyboard::Scancode k) {
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

int Menu::manageOtherEvents() {
    if (const std::optional<sf::Event> event = getEventForDerivedClasses(); event->getIf<sf::Event::Resized>()) {
        manageResizedEvent();
    } else if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
        manageKey(keyPressed->scancode);
    }
    return 0;
}

int Menu::draw() {
    window->draw(backgroundAndCursorVertexArray);
    text.draw();
    return 0;
}

Menu::Menu(sf::RenderWindow *window_, std::string parameters_, std::vector<int8_t> actions_,
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

