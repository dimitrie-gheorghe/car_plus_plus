//
// Created by dimitrie on 20.06.2026.
//

#ifndef OOP_TILEPANEL_H
#define OOP_TILEPANEL_H

#include <filesystem>
#include <memory>
#include <ranges>
#include <string>
#include <vector>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "../ext/portable-file-dialogs.h"
#include "./ReadOnlyText.h"
#include "./SceneID.h"
#include "./Settings.h"
#include "./Scene.h"

class TilePanel : public Scene {
    std::string titleText;
    const uint8_t target;
    ReadOnlyText titleObject;

    int8_t cursorX = 0;
    int8_t cursorY = 0;
    const int16_t x = 0;
    int16_t y = static_cast<int16_t>(Settings::getInstance().pixel_size() * 9);
    int16_t cellW = 0;
    int16_t cellH = 0;
    int columns = 0;
    int rows = 0;
    std::vector<sf::Color> colors;
    sf::VertexArray tableVertexArray;
    sf::VertexArray cursorVertexArray;

    void createTilePanel();

    void manageResizedEvent();

    void updateCursor();

    void reset() override;

    void manageEvent() override;

public:
    [[nodiscard]] std::unique_ptr<Scene> clone() const override;

    friend std::ostream &operator<<(std::ostream &os, const TilePanel &obj);

    std::ostream &print(std::ostream &os) const override;

    TilePanel(sf::RenderWindow &window_, std::string title_, uint8_t target_, SceneID sceneToReturnAt_);

    void begin() override;

    void draw() override;
};

#endif //OOP_TILEPANEL_H