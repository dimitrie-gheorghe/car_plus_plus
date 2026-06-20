//
// Created by dimitrie on 20.06.2026.
//

#ifndef OOP_MENU_H
#define OOP_MENU_H

#include <filesystem>
#include <memory>
#include <ranges>
#include <string>
#include <vector>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "./ReadOnlyText.h"
#include "./SceneID.h"
#include "./TilePanel.h"

class Menu : public Scene {
    ReadOnlyText text;
    std::vector<SceneID> actions;
    int cursor = 0;
    sf::VertexArray cursorVertexArray;
    int len = 0;
    int16_t lineHeight = 0;
    std::string parameters;

    void updateCursor();

    void reset() override;

    void manageEvent() override;

public:
    [[nodiscard]] std::unique_ptr<Scene> clone() const override;

    friend std::ostream &operator<<(std::ostream &os, const Menu &obj);

    std::ostream &print(std::ostream &os) const override;

    Menu(sf::RenderWindow &window_, const std::string &menuText, const std::vector<SceneID> &actions_,
         const SceneID sceneToReturnAt_);

    void draw() override;

    void begin() override;
};


#endif //OOP_MENU_H