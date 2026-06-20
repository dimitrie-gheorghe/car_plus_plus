//
// Created by dimitrie on 20.06.2026.
//

#include "../include/Greet.h"

#include <memory>
#include <ranges>
#include <string>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "../ext/portable-file-dialogs.h"
#include "../include/ReadOnlyText.h"
#include "../include/SceneID.h"
#include "../include/Settings.h"
#include "../include/TilePanel.h"


void Greet::setProperties() {
    formerWindowW = window.getSize().x;
    formerWindowH = window.getSize().y;
    formerPos = window.getPosition();

    sf::Vector2i desiredPos;
    desiredPos.x = static_cast<int>(formerPos.x + (window.getSize().x - W) / 2);
    desiredPos.y = static_cast<int>(formerPos.y + (window.getSize().y - H) / 2);
    window.create(sf::VideoMode({static_cast<unsigned int>(W), static_cast<unsigned int>(H)}), "", sf::Style::None,
                  sf::State::Windowed);
    window.setPosition(desiredPos);
    window.setFramerateLimit(30);
}

void Greet::resetProperties() {
    window.create(sf::VideoMode({formerWindowW, formerWindowH}), "car_plus_plus", sf::Style::Default,
                  sf::State::Windowed);
    window.setFramerateLimit(10);
}

void Greet::reset() {
    x = static_cast<int16_t>(W);
    greeting.moveAt(x, 0);
    resetProperties();
}

[[nodiscard]] std::unique_ptr<Scene> Greet::clone() const {
    return std::make_unique<Greet>(*this);
}


Greet::Greet(sf::RenderWindow &window_, const std::string &s, const SceneID sceneToReturnAt_)
    : Scene(window_, sceneToReturnAt_) {
    textSize = s.size();
    std::string backgroundText;
    constexpr uint8_t backgroundTextLen = 8 * 8 * 3;
    uint8_t i = 0;
    while (i < backgroundTextLen) {
        for (char c = ' '; i < backgroundTextLen && c <= '~'; c++) {
            backgroundText += c;
            i++;
        }
    }
    matrix = ReadOnlyText{&window, backgroundText, 0, 0, pixelWidth, W, H, Settings::getInstance().text_color()};
    greeting = ReadOnlyText{
        &window, s, static_cast<int16_t>(W), 0, static_cast<uint8_t>(pixelWidth * 9),
        9 * 9 * pixelWidth * textSize, static_cast<size_t>(9 * 8 * pixelWidth),
        Settings::getInstance().text_color(), false
    };
    x = static_cast<int16_t>(W);
}

void Greet::draw() {
    if (x < -static_cast<int16_t>(9 * 8 * pixelWidth * textSize)) {
        resetProperties();
        exit();
    }
    matrix.draw();
    greeting.draw();
    x = static_cast<int16_t>(x - 9 * pixelWidth);
    greeting.moveAt(x, 0);
}

void Greet::begin() {
    setProperties();
}

void Greet::manageEvent() {
}

std::ostream &operator<<(std::ostream &os, const Greet &obj) {
    return obj.print(os);
}

std::ostream &Greet::print(std::ostream &os) const {
    Scene::print(os);
    os << " | Derived Greet Panel -> W: " << W << ", H: " << H << ", textSize: " << textSize;
    return os;
}
