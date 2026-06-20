//
// Created by dimitrie on 20.06.2026.
//

#include "../include/Menu.h"
#include <functional>
#include <memory>
#include <ranges>
#include <string>
#include <vector>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "../ext/portable-file-dialogs.h"
#include "../include/ReadOnlyText.h"
#include "../include/SceneID.h"
#include "../include/Settings.h"
#include "../include/VertexArrayUtility.h"
#include "../include/TilePanel.h"

void Menu::updateCursor() {
    lineHeight = static_cast<int16_t>(Settings::getInstance().pixel_size() * 9);
    cursorVertexArray.clear();
    VertexArrayUtility::insertRectangle(cursorVertexArray, 0, static_cast<int16_t>((cursor + 1) * lineHeight),
                                        static_cast<int16_t>(window.getSize().x), lineHeight,
                                        Settings::getInstance().cursor_color());
}

void Menu::manageEvent() {
    if (!event) {
        return;
    }
    if (event->is<sf::Event::Resized>()) {
        text.manageResizedEvent();
        updateCursor();
        return;
    }
    if (const auto *key = event->getIf<sf::Event::KeyPressed>()) {
        switch (key->scancode) {
            case sf::Keyboard::Scancode::Up:
            case sf::Keyboard::Scancode::Left:
                cursor--;
                if (cursor < 0) {
                    cursor = len - 1;
                }
                updateCursor();
                break;
            case sf::Keyboard::Scancode::Down:
            case sf::Keyboard::Scancode::Right:
                cursor++;
                if (cursor >= len) {
                    cursor = 0;
                }
                updateCursor();
                break;
            case sf::Keyboard::Scancode::Enter:
                if (cursor >= 0 && cursor < len) {
                    setSceneToReturnAt(actions[cursor]);
                    exit();
                }
                break;
            case sf::Keyboard::Scancode::Escape:
                exit();
                break;
            default: break;
        }
    }
}

[[nodiscard]] std::unique_ptr<Scene> Menu::clone() const {
    return std::make_unique<Menu>(*this);
}

Menu::Menu(sf::RenderWindow &window_, const std::string &menuText, const std::vector<SceneID> &actions_,
           const SceneID sceneToReturnAt_)
    : Scene(window_, sceneToReturnAt_), actions(actions_) {
    len = static_cast<int>(actions.size());
    text = ReadOnlyText{
        &window, menuText, 0, 0, static_cast<uint8_t>(Settings::getInstance().pixel_size()),
        static_cast<size_t>(Settings::getInstance().window_w()),
        static_cast<size_t>(Settings::getInstance().window_h()), Settings::getInstance().text_color(), false
    };
    cursorVertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);
    updateCursor();
}

void Menu::draw() {
    window.draw(cursorVertexArray);
    text.draw();
}

void Menu::begin() {
    text.setTextColor(Settings::getInstance().text_color());
    text.setPixelSize(Settings::getInstance().pixel_size());
    text.refresh();
    updateCursor();
}

void Menu::reset() {
}

std::ostream &operator<<(std::ostream &os, const Menu &obj) {
    return obj.print(os);
}

std::ostream &Menu::print(std::ostream &os) const {
    Scene::print(os);
    os << " | Derived Menu -> Active components count: " << len << ", selected index: " << cursor;
    return os;
}
