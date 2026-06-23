//
// Created by dimitrie on 23.06.2026.
//

#include "../include/ThemeFactory.h"
#include "SFML/Graphics/Color.hpp"

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
                sf::Color(0, 255, 0)
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
                sf::Color(20, 5, 30),
                sf::Color(255, 0, 128),
                sf::Color(0, 255, 255)
            };
        }
    }
    return {
        sf::Color::White,
        sf::Color::Black,
        sf::Color::Green
    };
}