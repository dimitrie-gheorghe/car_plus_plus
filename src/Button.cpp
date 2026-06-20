//
// Created by dimitrie on 20.06.2026.
//

#include "../include/Button.h"
#include <functional>
#include <memory>
#include "../include/TilePanel.h"

[[nodiscard]] std::unique_ptr<Scene> Button::clone() const {
    return std::make_unique<Button>(*this);
}

void Button::begin() {
    action();
    exit();
}

std::ostream &Button::print(std::ostream &os) const {
    Scene::print(os);
    os << " | Derived Button -> (System functional trigger tile)";
    return os;
}

void Button::reset() {
}

void Button::manageEvent() {
}

std::ostream &operator<<(std::ostream &os, const Button &obj) {
    return obj.print(os);
}

Button::Button(sf::RenderWindow &window, std::function<void()> func, const SceneID sceneToReturnAt_)
    : Scene(window, sceneToReturnAt_), action(std::move(func)) {
}

void Button::draw() {
}
