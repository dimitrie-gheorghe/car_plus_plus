//
// Created by dimitrie on 20.06.2026.
//

#ifndef OOP_SCENE_H
#define OOP_SCENE_H

#include <filesystem>
#include <memory>
#include <ranges>
#include <SFML/Graphics.hpp>
#include "./SceneID.h"

class Scene {
    virtual void manageEvent() = 0;

    virtual void reset() = 0;

    bool requestExit = false;
    SceneID sceneToReturnAt = SceneID::Exit;
    SceneID defaultSceneToReturnAt = SceneID::Exit;

protected:
    sf::RenderWindow &window;
    std::optional<sf::Event> event;

    Scene(const Scene &other) = default;

    Scene(Scene &&other) noexcept;

    Scene &operator=(const Scene &other);

    Scene &operator=(Scene &&other) noexcept;

public:
    virtual ~Scene() = default;

    friend std::ostream &operator<<(std::ostream &os, const Scene &obj);

    virtual std::ostream &print(std::ostream &os) const;

    explicit Scene(sf::RenderWindow &w, SceneID sceneToReturnAt_);

    [[nodiscard]] virtual std::unique_ptr<Scene> clone() const = 0;

    virtual void draw() = 0;

    virtual void begin() = 0;

    virtual void resume();

    void end();

    void manageEvent(const sf::Event &e);

    void setSceneToReturnAt(const SceneID id);

    [[nodiscard]] SceneID getSceneToReturnAt() const;

    [[nodiscard]] bool hasFinished() const;

    void exit();
};


#endif //OOP_SCENE_H
