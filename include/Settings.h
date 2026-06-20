//
// Created by dimitrie on 20.06.2026.
//

#ifndef OOP_SETTINGS_H
#define OOP_SETTINGS_H

#include <filesystem>
#include <sstream>
#include <SFML/Graphics.hpp>

class Settings {
    int16_t windowW = 800;
    int16_t windowH = 700;
    int16_t pixelSize = 2;

    sf::Color backgroundColor = sf::Color(255, 255, 255);
    sf::Color textColor = sf::Color(0, 0, 0);
    sf::Color cursorColor = sf::Color(0, 255, 0);

    bool isEnglish = true;

    void loadSettings(bool resetDefaults = false);

    void storeSettings() const;

public:
    friend std::ostream &operator<<(std::ostream &os, const Settings &obj);

    Settings();

    Settings(const Settings &) = delete;

    Settings &operator=(const Settings &) = delete;

    static Settings &getInstance();

    virtual ~Settings();

    void update(bool resetDefaults = false);

    [[nodiscard]] bool getIsEnglish() const;

    void toggleLanguage();

    [[nodiscard]] int16_t window_w() const;

    [[nodiscard]] int16_t window_h() const;

    [[nodiscard]] int16_t pixel_size() const;

    [[nodiscard]] sf::Color background_color() const;

    [[nodiscard]] sf::Color text_color() const;

    [[nodiscard]] sf::Color cursor_color() const;

    void increase_pixel_size();

    void decrease_pixel_size();

    void set_background_color(const sf::Color &background_color);

    void set_text_color(const sf::Color &text_color);

    void set_cursor_color(const sf::Color &cursor_color);
};

#endif //OOP_SETTINGS_H
