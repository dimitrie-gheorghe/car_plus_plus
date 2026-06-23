//
// Created by dimitrie on 20.06.2026.
//

#ifndef OOP_EDITABLETEXT_H
#define OOP_EDITABLETEXT_H

#include <filesystem>
#include <memory>
#include <ranges>
#include <string>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "../ext/portable-file-dialogs.h"
#include "./ReadOnlyText.h"
#include "./SceneID.h"
#include "./TilePanel.h"

class EditableText : public Scene {
    std::string text;
    std::filesystem::path currentFilePath;

    int16_t x = 0;
    int16_t y = 0;
    int16_t initialY = 0;
    size_t W = 0;
    size_t H = 0;
    sf::VertexArray textVertexArray;

    size_t cursorL = 0;
    size_t cursorR = 0;
    size_t *selectionHead = &cursorR;
    size_t step = 0;
    bool ctrl = false;
    bool shift = false;

    ReadOnlyText infoBanner;

    void updateVertexArray();

    void deleteSelectedText();

    void manageKey(sf::Keyboard::Scancode k);

    void reset() override;

    void manageEvent() override;

    void setupInfoBanner();

public:
    EditableText(const EditableText &other);

    EditableText(EditableText &&other) noexcept;

    EditableText &operator=(const EditableText &other);

    EditableText &operator=(EditableText &&other) noexcept;

    ~EditableText() override = default;

    [[nodiscard]] std::filesystem::path getFilePath() const;

    [[nodiscard]] std::string getText() const;

    [[nodiscard]] std::unique_ptr<Scene> clone() const override;

    friend std::ostream &operator<<(std::ostream &os, const EditableText &obj);

    std::ostream &print(std::ostream &os) const override;

    explicit EditableText(sf::RenderWindow &window_, std::string initialText_, std::filesystem::path filePath,
                          int16_t x_, int16_t y_, size_t W_, size_t H_,
                          SceneID sceneToReturnAt_);

    void draw() override;

    void begin() override;

};


#endif //OOP_EDITABLETEXT_H
