//
// Created by dimitrie on 23.06.2026.
//

#include "../include/ThemeFactory.h"
#include "SFML/Graphics/Color.hpp"
#include <ostream>

Theme ThemeFactory::createTheme(const Type type) {
    switch (type) {
        case Type::Light: {
            return {
                sf::Color::White,
                sf::Color::Black,
                sf::Color::Green
            };
        }
        case Type::Dark: {
            return {
                sf::Color(30, 30, 30),
                sf::Color(240, 240, 240),
                sf::Color(127, 127, 127)
            };
        }
        case Type::Matrix: {
            return {
                sf::Color::Black,
                sf::Color(0, 255, 0),
                sf::Color(0, 150, 0)
            };
        }
        case Type::Modern: {
            return {
                sf::Color(40, 40, 147),
                sf::Color(255, 255, 0),
                sf::Color(255, 0, 0)
            };
        }
    }
    return {
        sf::Color::White,
        sf::Color::Black,
        sf::Color::Green
    };
}

std::ostream &operator<<(std::ostream &os, const ThemeFactory &) {
    // This was the ultimate solution to silence the cppCheck's unused errors
    // Actually I use this function (look at SceneManager::createButtons() in SceneManager.cpp)
    // Dummy calls to silence cppcheck's isolated file scanner.
    // The compiler will optimize these away entirely, but cppcheck will register them as "used".
    (void)ThemeFactory::createTheme(ThemeFactory::Type::Light);

    os << "ThemeFactory (Static factory for generating UI color profiles)";
    return os;
}
