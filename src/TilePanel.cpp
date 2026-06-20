//
// Created by dimitrie on 20.06.2026.
//

#include "../include/TilePanel.h"
#include <utility>
#include <functional>
#include <filesystem>
#include <memory>
#include <fstream>
#include <ranges>
#include <string>
#include <vector>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include "../ext/portable-file-dialogs.h"
#include "../include/AppExceptions.h"
#include "../include/ReadOnlyText.h"
#include "../include/SceneID.h"
#include "../include/Settings.h"
#include "../include/VertexArrayUtility.h"

void TilePanel::createTilePanel() {
    y = static_cast<int16_t>(Settings::getInstance().pixel_size() * 9);
    tableVertexArray.clear();
    tableVertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);
    cursorVertexArray.clear();
    cursorVertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);

    const std::filesystem::path sourcePath = SOURCE_DIR;
    const std::filesystem::path dataPath = sourcePath / "assets" / "tilePanelData.txt";

    std::ifstream input(dataPath);
    if (!input.is_open()) {
        throw AssetLoadError();
    }
    input >> rows >> columns;

    cellW = static_cast<int16_t>(window.getSize().x / (2 * columns));
    cellH = static_cast<int16_t>(window.getSize().y / (2 * rows));

    for (int i = 0; i <= rows; i++) {
        VertexArrayUtility::insertRectangle(tableVertexArray, x, static_cast<int16_t>(y + cellH * i),
                                            static_cast<int16_t>(cellW * columns + 10), 10,
                                            Settings::getInstance().text_color());
    }
    for (int i = 0; i <= columns; i++) {
        VertexArrayUtility::insertRectangle(tableVertexArray, static_cast<int16_t>(x + cellW * i), y, 10,
                                            static_cast<int16_t>(cellH * rows),
                                            Settings::getInstance().text_color());
    }

    int r, g, b;
    for (int j = 0; j < rows; j++) {
        for (int i = 0; i < columns; i++) {
            input >> r >> g >> b;
            colors.emplace_back(r, g, b);
            VertexArrayUtility::insertRectangle(tableVertexArray, static_cast<int16_t>(x + 10 + cellW * i),
                                                static_cast<int16_t>(y + 10 + cellH * j),
                                                static_cast<int16_t>(cellW - 10), static_cast<int16_t>(cellH - 10),
                                                sf::Color(r, g, b));
        }
    }
    input.close();

    cursorVertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);
    for (int i = 0; i <= 1; i++) {
        VertexArrayUtility::insertRectangle(cursorVertexArray, x, static_cast<int16_t>(y + cellH * i),
                                            static_cast<int16_t>(cellW + 10), 10,
                                            Settings::getInstance().cursor_color());
    }
    for (int i = 0; i <= 1; i++) {
        VertexArrayUtility::insertRectangle(cursorVertexArray, static_cast<int16_t>(x + cellW * i), y, 10, cellH,
                                            Settings::getInstance().cursor_color());
    }
}

void TilePanel::manageResizedEvent() {
    titleObject.manageResizedEvent();
}

void TilePanel::updateCursor() {
    VertexArrayUtility::moveAt(cursorVertexArray, static_cast<int16_t>(x + cellW * cursorX),
                               static_cast<int16_t>(y + cellH * cursorY));
}

void TilePanel::reset() {
    cursorX = 0;
    cursorY = 0;
}

void TilePanel::manageEvent() {
    if (!event) {
        return;
    }
    if (event->is<sf::Event::Resized>()) {
        manageResizedEvent();
        return;
    }
    if (const auto *key = event->getIf<sf::Event::KeyPressed>()) {
        switch (key->scancode) {
            case sf::Keyboard::Scancode::Right: cursorX++;
                break;
            case sf::Keyboard::Scancode::Left: cursorX--;
                break;
            case sf::Keyboard::Scancode::Up: cursorY--;
                break;
            case sf::Keyboard::Scancode::Down: cursorY++;
                break;
            case sf::Keyboard::Scancode::Enter: {
                if (const sf::Color newColor = colors[cursorX + cursorY * columns];
                    newColor != Settings::getInstance().background_color() && newColor != Settings::getInstance().
                    text_color() && newColor != Settings::getInstance().cursor_color()) {
                    if (target == 0) {
                        Settings::getInstance().set_background_color(newColor);
                    }
                    if (target == 1) {
                        Settings::getInstance().set_text_color(newColor);
                    }
                    if (target == 2) {
                        Settings::getInstance().set_cursor_color(newColor);
                    }
                    Settings::getInstance().update();
                }
                setSceneToReturnAt(SceneID::SettingsMenu);
                exit();
                return;
            }
            case sf::Keyboard::Scancode::Escape: setSceneToReturnAt(SceneID::SettingsMenu);
                exit();
                return;
            default: break;
        }
        if (cursorX < 0) {
            cursorX = 0;
        }
        if (cursorY < 0) {
            cursorY = 0;
        }
        if (cursorX >= columns) {
            cursorX = static_cast<int8_t>(columns - 1);
        }
        if (cursorY >= rows) {
            cursorY = static_cast<int8_t>(rows - 1);
        }
        updateCursor();
    }
}

[[nodiscard]] std::unique_ptr<Scene> TilePanel::clone() const {
    return std::make_unique<TilePanel>(*this);
}

TilePanel::TilePanel(sf::RenderWindow &window_, std::string title_, const uint8_t target_,
                     const SceneID sceneToReturnAt_)
    : Scene(window_, sceneToReturnAt_), titleText(std::move(title_)), target(target_) {
    titleObject = ReadOnlyText{
        &window, titleText, 0, 0, static_cast<uint8_t>(Settings::getInstance().pixel_size()),
        static_cast<uint16_t>(Settings::getInstance().window_w()),
        static_cast<uint16_t>(Settings::getInstance().pixel_size() * 9), Settings::getInstance().text_color(), false
    };
    createTilePanel();
}

void TilePanel::begin() {
    titleObject.setTextColor(Settings::getInstance().text_color());
    titleObject.setPixelSize(static_cast<uint8_t>(Settings::getInstance().pixel_size()));
    titleObject.refresh();
    createTilePanel();
}

void TilePanel::draw() {
    titleObject.draw();
    window.draw(tableVertexArray);
    window.draw(cursorVertexArray);
}

std::ostream &TilePanel::print(std::ostream &os) const {
    Scene::print(os);
    os << " | Derived TilePanel -> title: \"" << titleText << "\", Active grid structure: " << rows << "x" <<
            columns;
    return os;
}
