//
// Created by dimitrie on 23.05.2026.
//

#ifndef OOP_SCENE_H
#define OOP_SCENE_H


class Scene {
    sf::RenderWindow *windowUsedForMainLoop;
    std::optional<sf::Event> eventForDerivedClasses;

    virtual int manageOtherEvents() = 0;

    virtual int draw() = 0;

protected:
    Scene(const Scene &other) = default;

    Scene(Scene &&other) noexcept
        : windowUsedForMainLoop(other.windowUsedForMainLoop),
          eventForDerivedClasses(other.eventForDerivedClasses) {
    }

    Scene &operator=(const Scene &other) {
        if (this == &other)
            return *this;
        windowUsedForMainLoop = other.windowUsedForMainLoop;
        eventForDerivedClasses = other.eventForDerivedClasses;
        return *this;
    }

    Scene &operator=(Scene &&other) noexcept {
        if (this == &other)
            return *this;
        windowUsedForMainLoop = other.windowUsedForMainLoop;
        eventForDerivedClasses = other.eventForDerivedClasses;
        return *this;
    }

public:
    Scene() {
        windowUsedForMainLoop = nullptr;
    }

    explicit Scene(sf::RenderWindow *window_) {
        windowUsedForMainLoop = window_;
    }

    [[nodiscard]] virtual Scene *clone() const = 0;

    virtual ~Scene() = default;

    [[nodiscard]] std::optional<sf::Event> getEventForDerivedClasses() const {
        return eventForDerivedClasses;
    }

    int play();
};

#endif //OOP_SCENE_H