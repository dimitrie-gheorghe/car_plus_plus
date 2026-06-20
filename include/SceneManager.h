//
// Created by dimitrie on 20.06.2026.
//

#ifndef OOP_SCENEMANAGER_H
#define OOP_SCENEMANAGER_H

#include <memory>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "./SceneID.h"
#include "./TilePanel.h"

class Scene;

class SceneManager {
    std::unordered_map<SceneID, std::unique_ptr<Scene> > scenes;
    sf::VertexArray background;
    Scene *currentScene = nullptr;

    sf::RenderWindow window;

    void updateBackground();

    bool handleFileOperation(SceneID action);

    [[nodiscard]] Scene *resolve(const SceneID id);

    void createButtons();

    void createScenes();

public:
    friend std::ostream &operator<<(std::ostream &os, const SceneManager &obj);

    explicit SceneManager();

    void run();
};

#endif //OOP_SCENEMANAGER_H
