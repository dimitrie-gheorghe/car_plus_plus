//
// Created by dimitrie on 20.06.2026.
//

#include "../include/SceneManager.h"
#include <utility>
#include <functional>
#include <filesystem>
#include <memory>
#include <iostream>
#include <fstream>
#include <ostream>
#include <ranges>
#include <string>
#include <vector>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "../ext/portable-file-dialogs.h"
#include "../include/AppExceptions.h"
#include "../include/EditableText.h"
#include "../include/Greet.h"
#include "../include/SceneID.h"
#include "../include/Settings.h"
#include "../include/VertexArrayUtility.h"
#include "../include/Button.h"
#include "../include/Menu.h"
#include "../include/TilePanel.h"
#include "../include/Scene.h"

void SceneManager::updateBackground() {
    background.clear();
    VertexArrayUtility::insertRectangle(background, 0, 0, static_cast<int16_t>(window.getSize().x),
                                        static_cast<int16_t>(window.getSize().y),
                                        Settings::getInstance().background_color());
}

bool SceneManager::handleFileOperation(SceneID action) {
    std::filesystem::path targetPath;
    std::string content;
    if (action == SceneID::OpenFromDisk) {
        auto selection = pfd::open_file("Open Text File", ".", {"Text Files", "*.txt", "All Files", "*"}).result();
        if (selection.empty()) {
            return false;
        }
        targetPath = selection[0];
        if (std::ifstream input(targetPath); input.is_open()) {
            content.assign((std::istreambuf_iterator(input)), std::istreambuf_iterator<char>());
        } else {
            pfd::message("Error", "Could not read the file.", pfd::choice::ok, pfd::icon::error).result();
            return false;
        }
    } else if (action == SceneID::NewFile) {
        auto destination = pfd::save_file("Create New File", ".", {"Text Files", "*.txt"}).result();
        if (destination.empty()) {
            return false;
        }
        targetPath = destination;
    }
    scenes[SceneID::TextEditor] = std::make_unique<EditableText>(window, content, targetPath, 0, 0,
                                                                 window.getSize().x, window.getSize().y,
                                                                 SceneID::MainMenu);
    return true;
}

[[nodiscard]] Scene *SceneManager::resolve(const SceneID id) {
    if (id == SceneID::Exit) {
        window.close();
        return nullptr;
    }
    if (id == SceneID::NewFile || id == SceneID::OpenFromDisk) {
        return nullptr;
    }
    if (scenes.contains(id)) {
        return scenes.at(id).get();
    }
    throw SceneInitializationError();
}

void SceneManager::createButtons() {
    std::unordered_map<SceneID, std::pair<std::function<void()>, SceneID> > buttonRegistry = {
        {
            SceneID::Increase, {
                [] {
                    Settings::getInstance().increase_pixel_size();
                    Settings::getInstance().update();
                },
                SceneID::PixelSizeSettings
            }
        },
        {
            SceneID::Decrease, {
                [] {
                    Settings::getInstance().decrease_pixel_size();
                    Settings::getInstance().update();
                },
                SceneID::PixelSizeSettings
            }
        },
        {
            SceneID::RestoreDefaults, {
                [this] {
                    Settings::getInstance().update(true);
                    this->createScenes();
                },
                SceneID::SettingsMenu
            }
        },
        {
            SceneID::ToggleLanguage, {
                [this] {
                    Settings::getInstance().toggleLanguage();
                    Settings::getInstance().update();
                    this->createScenes();
                },
                SceneID::SettingsMenu
            }
        }
    };
    for (auto &[id, config]: buttonRegistry)
        scenes[id] = std::make_unique<Button>(
            window, config.first, config.second);
}

void SceneManager::createScenes() {
    bool eng = Settings::getInstance().getIsEnglish();
    std::string bgTitle = eng ? "Background: Enter to select" : "Fundal: Enter pentru selectie";
    std::string textTitle = eng ? "Text: Enter to select" : "Text: Enter pentru selectie";
    std::string cursorTitle = eng ? "Cursor: Enter to select" : "Cursor: Enter pentru selectie";
    std::string aiTitle = eng ? "malware successfully installed" : "malware instalat cu succes";

    scenes[SceneID::BackgroundSettings] = std::make_unique<TilePanel>(window, bgTitle, 0, SceneID::SettingsMenu);
    scenes[SceneID::TextSettings] = std::make_unique<TilePanel>(window, textTitle, 1, SceneID::SettingsMenu);
    scenes[SceneID::CursorSettings] = std::make_unique<TilePanel>(window, cursorTitle, 2, SceneID::SettingsMenu);
    scenes[SceneID::AIMode] = std::make_unique<Greet>(window, aiTitle, SceneID::Exit);

    std::string filename = eng ? "menu_eng.txt" : "menu_ro.txt";
    const std::filesystem::path menuPath = std::filesystem::current_path() / "config" / filename;
    std::ifstream menuFile(menuPath);

    if (!menuFile.is_open()) {
        throw ConfigurationError();
    }

    std::string line;
    int currentMenuID = -1;
    int returnID = 13;
    std::string menuText;
    std::vector<SceneID> actions;
    auto buildActiveMenu = [&] {
        if (currentMenuID == -1) {
            return;
        }
        if (const auto menuID = static_cast<SceneID>(currentMenuID); menuID == SceneID::Intro) {
            scenes[menuID] = std::make_unique<Greet>(window, menuText, static_cast<SceneID>(returnID));
        } else {
            scenes[menuID] = std::make_unique<Menu>(window, menuText, actions, static_cast<SceneID>(returnID));
        }
        actions.clear();
        menuText.clear();
        currentMenuID = -1;
    };

    while (std::getline(menuFile, line)) {
        if (line.empty()) {
            continue;
        }
        if (line == "[Menu]") {
            buildActiveMenu();
            continue;
        }
        size_t delimiterPos = line.find(':');
        if (delimiterPos == std::string::npos) {
            continue;
        }
        std::string key = line.substr(0, delimiterPos);
        std::string value = line.substr(delimiterPos + 1);
        value.erase(0, value.find_first_not_of(" \t"));

        if (key == "ID") {
            currentMenuID = std::stoi(value);
        } else if (key == "Return") {
            returnID = std::stoi(value);
        } else if (key == "Title") {
            menuText = value;
        } else if (key == "Item") {
            if (size_t arrowPos = value.find("->"); arrowPos != std::string::npos) {
                std::string label = value.substr(0, arrowPos);
                std::string targetSceneStr = value.substr(arrowPos + 2);
                label.erase(label.find_last_not_of(" \t") + 1);
                menuText += "\n" + label;
                actions.push_back(static_cast<SceneID>(std::stoi(targetSceneStr)));
            }
        }
    }
    buildActiveMenu();
    menuFile.close();
}


SceneManager::SceneManager() {
    window.create(sf::VideoMode({
                      static_cast<uint16_t>(Settings::getInstance().window_w()),
                      static_cast<uint16_t>(Settings::getInstance().window_h())
                  }), "car_plus_plus", sf::Style::Default, sf::State::Windowed);
    std::cout << "The window was created successfully\n";
    window.setFramerateLimit(10);

    background.setPrimitiveType(sf::PrimitiveType::Triangles);
    updateBackground();
    createButtons();
    createScenes();
    currentScene = scenes[SceneID::Intro].get();
}

void SceneManager::run() {
    if (currentScene) {
        currentScene->begin();
    }
    while (window.isOpen()) {
        while (auto e = window.pollEvent()) {
            if (e->is<sf::Event::Closed>()) {
                window.close();
                return;
            }
            if (const auto *resized = e->getIf<sf::Event::Resized>()) {
                sf::FloatRect visibleArea({0.f, 0.f}, sf::Vector2f(resized->size));
                window.setView(sf::View(visibleArea));

                updateBackground();
            }
            currentScene->manageEvent(*e);
        }

        window.clear();
        window.draw(background);
        currentScene->draw();
        window.display();

        if (currentScene->hasFinished()) {
            if (const auto *editor = dynamic_cast<EditableText *>(currentScene)) {
                bool eng = Settings::getInstance().getIsEnglish();
                std::string title = eng ? "Save Changes" : "Salveaza Modificarile";
                std::string message = eng
                                          ? "Save changes before closing?"
                                          : "Salvati modificarile inainte de a inchide?";
                auto box = pfd::message(title, message, pfd::choice::yes_no_cancel, pfd::icon::question);
                auto result = box.result();

                if (result == pfd::button::cancel) {
                    currentScene->resume();
                    continue;
                }
                if (result == pfd::button::yes) {
                    if (std::ofstream output(editor->getFilePath()); output.is_open()) output << editor->getText();
                }
            }

            updateBackground();
            SceneID next = currentScene->getSceneToReturnAt();
            currentScene->end();
            if (next == SceneID::NewFile || next == SceneID::OpenFromDisk) {
                if (handleFileOperation(next)) next = SceneID::TextEditor;
                else next = SceneID::MainMenu;
            }

            Scene *nextPtr = resolve(next);
            if (!nextPtr) {
                if (!window.isOpen()) {
                    return;
                }
                continue;
            }
            currentScene = nextPtr;
            currentScene->begin();
        }
    }
}

std::ostream &operator<<(std::ostream &os, const SceneManager &obj) {
    os << "\n======================================================================="
            << "\nThis is the SceneManager class"
            << "\n======================================================================="
            << "\nWindow: " << &obj.window
            << "\n-----------------------------------------------------------------------"
            << "\nCurrent scene:\n  ";
    if (obj.currentScene) {
        os << *obj.currentScene;
    } else {
        os << "None";
    }
    os << "\n-----------------------------------------------------------------------"
            << "\nScenes:";
    for (const auto &scene: obj.scenes | std::views::values) {
        os << "\n  -> ";
        if (scene) {
            os << *scene;
        } else os << "nullptr";
    }
    os << "\n=======================================================================";
    return os;
}
