//
// Created by dimitrie on 20.06.2026.
//

#include "../include/Scene.h"
#include <ranges>
#include <SFML/Graphics.hpp>
#include "../include/SceneID.h"

Scene::Scene(sf::RenderWindow &w, const SceneID sceneToReturnAt_)
    : sceneToReturnAt(sceneToReturnAt_), defaultSceneToReturnAt(sceneToReturnAt_), window(w) {
}

void Scene::resume() {
    requestExit = false;
}

void Scene::end() {
    requestExit = false;
    sceneToReturnAt = defaultSceneToReturnAt;
    reset();
}

void Scene::manageEvent(const sf::Event &e) {
    event = e;
    manageEvent();
}

void Scene::setSceneToReturnAt(const SceneID id) {
    sceneToReturnAt = id;
}

[[nodiscard]] SceneID Scene::getSceneToReturnAt() const {
    return sceneToReturnAt;
}

[[nodiscard]] bool Scene::hasFinished() const {
    return requestExit;
}

void Scene::exit() {
    requestExit = true;
}

Scene::Scene(Scene &&other) noexcept
    : requestExit(other.requestExit), sceneToReturnAt(other.sceneToReturnAt),
      defaultSceneToReturnAt(other.defaultSceneToReturnAt), window(other.window), event(other.event) {
}

Scene &Scene::operator=(const Scene &other) {
    if (this == &other) return *this;
    requestExit = other.requestExit;
    sceneToReturnAt = other.sceneToReturnAt;
    defaultSceneToReturnAt = other.defaultSceneToReturnAt;
    event = other.event;
    return *this;
}

Scene &Scene::operator=(Scene &&other) noexcept {
    if (this == &other) return *this;
    requestExit = other.requestExit;
    sceneToReturnAt = other.sceneToReturnAt;
    defaultSceneToReturnAt = other.defaultSceneToReturnAt;
    event = other.event;
    return *this;
}

std::ostream &operator<<(std::ostream &os, const Scene &obj) {
    return obj.print(os);
}

std::ostream &Scene::print(std::ostream &os) const {
    os << "Base Scene Data -> window pointer: " << &window << ", requestExit status: " << (
        requestExit ? "true" : "false");
    return os;
}
