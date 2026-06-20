//
// Created by dimitrie on 20.06.2026.
//

#ifndef OOP_GREET_H
#define OOP_GREET_H

#include <filesystem>
#include <memory>
#include <ranges>
#include <string>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "../ext/portable-file-dialogs.h"
#include "./ReadOnlyText.h"
#include "./SceneID.h"
#include "./TilePanel.h"

class Greet : public Scene {
    sf::Vector2i formerPos;
    unsigned int formerWindowW = 0;
    unsigned int formerWindowH = 0;
    uint8_t pixelWidth = 3;
    const size_t W = pixelWidth * 9 * 8 * 3;
    const size_t H = pixelWidth * 9 * 8;
    size_t textSize = 0;
    ReadOnlyText matrix;
    ReadOnlyText greeting;
    int16_t x = 0;

    void setProperties();

    void resetProperties();

    void reset() override;

    void manageEvent() override;

public:
    [[nodiscard]] std::unique_ptr<Scene> clone() const override;

    friend std::ostream &operator<<(std::ostream &os, const Greet &obj);

    std::ostream &print(std::ostream &os) const override;

    explicit Greet(sf::RenderWindow &window_, const std::string &s, const SceneID sceneToReturnAt_);

    void draw() override;

    void begin() override;
};


#endif //OOP_GREET_H