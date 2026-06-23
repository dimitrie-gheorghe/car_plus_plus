//
// Created by dimitrie on 23.06.2026.
//

#ifndef OOP_THEME_H
#define OOP_THEME_H

#include <iosfwd>
#include "SFML/Graphics/Color.hpp"

struct Theme {
    sf::Color background;
    sf::Color text;
    sf::Color cursor;
};

class ThemeFactory {
public:
    enum class Type {
        Light,
        Dark,
        Matrix,
        Modern
    };

    static Theme createTheme(Type type);

    friend std::ostream &operator<<(std::ostream &os, const ThemeFactory &tf);
};

#endif //OOP_THEME_H
