//
// Created by dimitrie on 20.06.2026.
//

#ifndef OOP_BUTTON_H
#define OOP_BUTTON_H

#include <functional>
#include <filesystem>
#include <memory>
#include <SFML/Graphics.hpp>
#include "./SceneID.h"
#include "./TilePanel.h"

class Button : public Scene {
    std::function<void()> action;

    void reset() override;

    void manageEvent() override;

public:
    [[nodiscard]] std::unique_ptr<Scene> clone() const override;

    friend std::ostream &operator<<(std::ostream &os, const Button &obj);

    std::ostream &print(std::ostream &os) const override;

    Button(sf::RenderWindow &window, std::function<void()> func, SceneID sceneToReturnAt_);

    void draw() override;

    void begin() override;
};


#endif //OOP_BUTTON_H