//
// Created by dimitrie on 20.06.2026.
//

#include "../include/Settings.h"

#include <functional>
#include <filesystem>
#include <memory>
#include <iostream>
#include <fstream>
#include <ostream>
#include <ranges>
#include <string>
#include <SFML/Graphics.hpp>
#include "../include/AppExceptions.h"

void Settings::loadSettings(const bool resetDefaults) {
    const std::filesystem::path settingsFile = std::filesystem::current_path() / "config" / "settings.txt";

    std::ifstream input(settingsFile);

    if (!input.is_open() && !resetDefaults) {
        throw ConfigurationError();
    }
    if (resetDefaults) {
        std::cout << "Default settings will be restored";
        windowW = 800;
        windowH = 700;
        pixelSize = 2;
        backgroundColor = sf::Color(255, 255, 255);
        textColor = sf::Color(0, 0, 0);
        cursorColor = sf::Color(0, 255, 0);
        isEnglish = true;
        return;
    }

    int r, g, b, x;
    input >> x;
    windowW = static_cast<int16_t>(x);
    input >> x;
    windowH = static_cast<int16_t>(x);
    input >> x;
    pixelSize = static_cast<int16_t>(x);

    input >> r >> g >> b;
    backgroundColor.r = r;
    backgroundColor.g = g;
    backgroundColor.b = b;
    input >> r >> g >> b;
    textColor.r = r;
    textColor.g = g;
    textColor.b = b;
    input >> r >> g >> b;
    cursorColor.r = r;
    cursorColor.g = g;
    cursorColor.b = b;

    if (std::string lang; input >> lang) {
        isEnglish = (lang == "eng");
    } else {
        isEnglish = true;
    }
    input.close();
}

void Settings::storeSettings() const {
    const std::filesystem::path settingsFile = std::filesystem::current_path() / "config" / "settings.txt";

    std::ofstream output(settingsFile);
    output << static_cast<int>(windowW) << " " << static_cast<int>(windowH) << "\n" << pixelSize
            << "\n" << static_cast<int>(backgroundColor.r) << " " << static_cast<int>(backgroundColor.g) << " " <<
            static_cast<int>(backgroundColor.b)
            << "\n" << static_cast<int>(textColor.r) << " " << static_cast<int>(textColor.g) << " " << static_cast<
                int>(textColor.b)
            << "\n" << static_cast<int>(cursorColor.r) << " " << static_cast<int>(cursorColor.g) << " " <<
            static_cast<int>(cursorColor.b)
            << "\n" << (isEnglish ? "eng" : "ro");
    output.close();
}

Settings::Settings() {
    loadSettings();
}

Settings &Settings::getInstance() {
    static Settings instance;
    return instance;
}

Settings::~Settings() {
    storeSettings();
}

void Settings::update(const bool resetDefaults) {
    if (resetDefaults) {
        loadSettings(resetDefaults);
        storeSettings();
    } else {
        storeSettings();
        loadSettings();
    }
}

[[nodiscard]] bool Settings::getIsEnglish() const {
    return isEnglish;
}

void Settings::toggleLanguage() {
    isEnglish = !isEnglish;
}

[[nodiscard]] int16_t Settings::window_w() const {
    return windowW;
}

[[nodiscard]] int16_t Settings::window_h() const {
    return windowH;
}

[[nodiscard]] int16_t Settings::pixel_size() const {
    return pixelSize;
}

[[nodiscard]] sf::Color Settings::background_color() const {
    return backgroundColor;
}

[[nodiscard]] sf::Color Settings::text_color() const {
    return textColor;
}

[[nodiscard]] sf::Color Settings::cursor_color() const {
    return cursorColor;
}

void Settings::increase_pixel_size() {
    if (pixelSize + 1 < 15) {
        pixelSize = static_cast<int16_t>(pixelSize + 1);
    }
}

void Settings::decrease_pixel_size() {
    if (pixelSize - 1 >= 1) {
        pixelSize = static_cast<int16_t>(pixelSize - 1);
    }
}

void Settings::set_background_color(const sf::Color &background_color) {
    backgroundColor = background_color;
}

void Settings::set_text_color(const sf::Color &text_color) {
    textColor = text_color;
}

void Settings::set_cursor_color(const sf::Color &cursor_color) {
    cursorColor = cursor_color;
}

std::ostream &operator<<(std::ostream &os, const Settings &obj) {
    os << "Settings -> windowW: " << obj.windowW << " windowH: " << obj.windowH
            << " pixelSize: " << obj.pixelSize << " backgroundColor: " << obj.backgroundColor.toInteger()
            << " textColor: " << obj.textColor.toInteger() << " cursorColor: " << obj.cursorColor.toInteger();
    return os;
}
