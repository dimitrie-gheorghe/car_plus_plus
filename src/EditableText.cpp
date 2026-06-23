//
// Created by dimitrie on 20.06.2026.
//

#include "../include/EditableText.h"
#include <functional>
#include <filesystem>
#include <memory>
#include <ranges>
#include <string>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include "../ext/portable-file-dialogs.h"
#include "../include/ReadOnlyText.h"
#include "../include/SceneID.h"
#include "../include/Settings.h"
#include "../include/VertexArrayUtility.h"
#include "../include/TilePanel.h"

void EditableText::updateVertexArray() {
    W = window.getSize().x;
    H = window.getSize().y - y;
    textVertexArray.clear();
    textVertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);

    std::string formattedText = text;
    const size_t charPoz = 9 * Settings::getInstance().pixel_size();
    const size_t rightLimit = W + x - charPoz;
    step = (W + x) / charPoz;

    size_t formattedCursorL = cursorL;
    size_t formattedCursorR = cursorR;
    size_t i = 0;
    while (i < formattedText.size()) {
        if (formattedText[i] == '\t') {
            if (i < formattedCursorL) formattedCursorL += 3;
            if (i < formattedCursorR) formattedCursorR += 3;
            formattedText[i] = ' ';
            formattedText.insert(i, "   ");
            i += 4;
        } else if (formattedText[i] == '\n') {
            const size_t old_i = i;
            formattedText[i] = ' ';
            i++;
            while (i % step) {
                formattedText.insert(i, " ");
                i++;
            }
            if (old_i < formattedCursorL) {
                formattedCursorL += i - old_i - 1;
            }
            if (old_i < formattedCursorR) {
                formattedCursorR += i - old_i - 1;
            }
        } else {
            i++;
        }
    }

    if (formattedCursorR == formattedText.size()) {
        formattedText += " ";
    }

    size_t visibleTextL = 0;
    size_t visibleTextR = formattedText.size();
    const size_t maxChars = (W * H) / (charPoz * charPoz);
    const size_t focusCursor = (selectionHead == &cursorL) ? formattedCursorL : formattedCursorR;

    while (focusCursor - visibleTextL + step >= maxChars) visibleTextL += step;
    if (visibleTextL + maxChars < visibleTextR) visibleTextR = visibleTextL + maxChars;

    for (size_t r = y; visibleTextL < visibleTextR; r += charPoz) {
        for (size_t c = x; c <= rightLimit && visibleTextL < visibleTextR; c += charPoz) {
            if (formattedCursorL <= visibleTextL && visibleTextL <= formattedCursorR) {
                VertexArrayUtility::insertRectangle(textVertexArray, static_cast<int16_t>(c),
                                                    static_cast<int16_t>(r),
                                                    static_cast<int16_t>(Settings::getInstance().pixel_size() * 9),
                                                    static_cast<int16_t>(Settings::getInstance().pixel_size() * 9),
                                                    Settings::getInstance().cursor_color());
            }
            if ('!' <= formattedText[visibleTextL] && formattedText[visibleTextL] <= '~') {
                VertexArrayUtility::insertChar(textVertexArray, formattedText[visibleTextL],
                                               static_cast<int16_t>(c), static_cast<int16_t>(r),
                                               Settings::getInstance().pixel_size(),
                                               Settings::getInstance().text_color());
            }
            visibleTextL++;
        }
    }
}

void EditableText::deleteSelectedText() {
    if (cursorL != cursorR) {
        text.erase(cursorL, cursorR - cursorL + 1);
        cursorR = cursorL;
    }
}

void EditableText::manageKey(const sf::Keyboard::Scancode k) {
    bool updateRequired = false;
    size_t newCursor = (selectionHead == &cursorR) ? cursorR : cursorL;

    if (k == sf::Keyboard::Scancode::Right) {
        if (*selectionHead + 1 <= text.size()) newCursor++;
    } else if (k == sf::Keyboard::Scancode::Left) {
        if (static_cast<int16_t>(*selectionHead - 1) >= 0) newCursor--;
    } else if (k == sf::Keyboard::Scancode::Up) {
        int left = static_cast<int>(newCursor);
        size_t l = 0;
        if (left >= 0) {
            left--;
            l++;
        }
        while (left >= 0 && text[left] != '\n') {
            left--;
            l++;
        }
        const size_t former_l = l;

        int tmpCursor = static_cast<int>(newCursor);
        for (uint8_t i = 0; i < 2; i++) {
            if (tmpCursor > -1) tmpCursor--;
            while (tmpCursor > -1 && text[tmpCursor] != '\n') tmpCursor--;
        }
        if (tmpCursor < static_cast<int>(text.size()) && l) {
            tmpCursor++;
            l--;
        }
        while (tmpCursor < static_cast<int>(text.size()) && text[tmpCursor] != '\n' && l) {
            tmpCursor++;
            l--;
        }
        if (tmpCursor + 1 == static_cast<int>(former_l) && tmpCursor == static_cast<int>(newCursor)) newCursor = 0;
        else newCursor = static_cast<size_t>(tmpCursor);
    } else if (k == sf::Keyboard::Scancode::Down) {
        int left = static_cast<int>(newCursor);
        size_t l = 0;
        if (left >= 0) {
            left--;
            l++;
        }
        while (left >= 0 && text[left] != '\n') {
            left--;
            l++;
        }

        if (text[newCursor] != '\n') {
            if (newCursor < text.size()) (newCursor)++;
            while (newCursor < text.size() && text[newCursor] != '\n') (newCursor)++;
        }
        if (newCursor < text.size() && l) {
            newCursor++;
            l--;
        }
        while (newCursor < text.size() && text[newCursor] != '\n' && l) {
            newCursor++;
            l--;
        }
    }

    if (newCursor != *selectionHead) {
        if (shift) {
            if (selectionHead == &cursorL) cursorL = newCursor;
            else cursorR = newCursor;
            if (cursorL > cursorR) {
                size_t _ = cursorL;
                cursorL = cursorR;
                cursorR = _;
                selectionHead = (selectionHead == &cursorL) ? &cursorR : &cursorL;
            }
        } else {
            if (cursorL == cursorR) {
                cursorL = newCursor;
                cursorR = newCursor;
            } else {
                if (k == sf::Keyboard::Scancode::Left || k == sf::Keyboard::Scancode::Up) cursorR = cursorL;
                else cursorL = cursorR;
            }
        }
        updateRequired = true;
    } else if (!shift) {
        if (k == sf::Keyboard::Scancode::Left || k == sf::Keyboard::Scancode::Up) {
            cursorR = cursorL;
            updateRequired = true;
        } else if (k == sf::Keyboard::Scancode::Right || k == sf::Keyboard::Scancode::Down) {
            cursorL = cursorR;
            updateRequired = true;
        }
    }

    if (!shift && ctrl) {
        if (k == sf::Keyboard::Scancode::X) {
            if (cursorL != cursorR) sf::Clipboard::setString(text.substr(cursorL, cursorR - cursorL + 1));
            deleteSelectedText();
            updateRequired = true;
        } else if (k == sf::Keyboard::Scancode::C) {
            if (cursorL != cursorR) sf::Clipboard::setString(text.substr(cursorL, cursorR - cursorL + 1));
        } else if (k == sf::Keyboard::Scancode::V) {
            const std::string s = sf::Clipboard::getString();
            if (!s.empty()) {
                deleteSelectedText();
                text.insert(cursorL, s);
                cursorL += s.size();
                cursorR = cursorL;
                updateRequired = true;
            }
        } else if (k == sf::Keyboard::Scancode::A) {
            cursorL = 0;
            cursorR = text.size();
            updateRequired = true;
        }
    }
    if (updateRequired) {
        updateVertexArray();
    }
}

void EditableText::manageEvent() {
    if (!event) {
        return;
    }
    if (const auto *enteredText = event->getIf<sf::Event::TextEntered>()) {
        if (const char c = static_cast<char>(enteredText->unicode); c == 8) {
            if (!text.empty()) {
                if (cursorL == cursorR && cursorL > 0) {
                    text.erase(--cursorL, 1);
                    cursorR = cursorL;
                } else {
                    deleteSelectedText();
                }
            }
        } else if ((c >= 32 && c < 127) || c == '\n' || c == '\t' || c == '\r') { // added \r for linux compliance
            deleteSelectedText();

            // Normalize OS-specific carriage returns to standard newlines
            // This is why enter didn't work on linux.
            // Now I also handle \r (converting it to \n in order to preserve current logic)
            char charToInsert = (c == '\r') ? '\n' : c;

            text.insert(cursorR++, 1, charToInsert);
            cursorL = cursorR;
        }
        updateVertexArray();
    } else if (event->getIf<sf::Event::Resized>()) {
        infoBanner.manageResizedEvent();
        updateVertexArray();
    } else if (const auto *key = event->getIf<sf::Event::KeyPressed>()) {
        if (key->scancode == sf::Keyboard::Scancode::Escape) {
            setSceneToReturnAt(SceneID::MainMenu);
            exit();
            return;
        }
        if (key->scancode == sf::Keyboard::Scancode::LControl || key->scancode == sf::Keyboard::Scancode::RControl)
            ctrl = true;
        if (key->scancode == sf::Keyboard::Scancode::LShift || key->scancode == sf::Keyboard::Scancode::RShift)
            shift = true;
        manageKey(key->scancode);
        updateVertexArray();
    } else if (const auto *k = event->getIf<sf::Event::KeyReleased>()) {
        if (k->scancode == sf::Keyboard::Scancode::LControl || k->scancode == sf::Keyboard::Scancode::RControl) {
            ctrl = false;
        }
        if (k->scancode == sf::Keyboard::Scancode::LShift || k->scancode == sf::Keyboard::Scancode::RShift) {
            shift = false;
        }
    }
}

void EditableText::setupInfoBanner() {
    const bool eng = Settings::getInstance().getIsEnglish();
    std::string bannerText = eng
                                 ? " ESC: Save Options\n------------------------------"
                                 : " ESC: Optiuni Salvare\n------------------------------";
    const size_t lineCount = std::ranges::count(bannerText, '\n') + 1;
    const auto bannerHeight = static_cast<int16_t>(Settings::getInstance().pixel_size() * 9 * lineCount);

    this->y = static_cast<int16_t>(initialY + bannerHeight);
    infoBanner = ReadOnlyText{
        &window, bannerText, x, initialY, static_cast<uint8_t>(Settings::getInstance().pixel_size()),
        window.getSize().x, static_cast<size_t>(bannerHeight), Settings::getInstance().text_color(), false
    };
}

[[nodiscard]] std::filesystem::path EditableText::getFilePath() const {
    return currentFilePath;
}

[[nodiscard]] std::string EditableText::getText() const {
    return text;
}

[[nodiscard]] std::unique_ptr<Scene> EditableText::clone() const {
    return std::make_unique<EditableText>(*this);
}

std::ostream &EditableText::print(std::ostream &os) const {
    Scene::print(os);
    os << " | Derived EditableText -> filePath: " << currentFilePath.string() << ", Total chars: " << text.size();
    return os;
}

void EditableText::draw() {
    infoBanner.draw();
    window.draw(textVertexArray);
}

void EditableText::begin() {
    setupInfoBanner();
    updateVertexArray();
}

void EditableText::reset() {
}

EditableText::EditableText(const EditableText &other)
    : Scene(other), text(other.text), currentFilePath(other.currentFilePath),
      x(other.x), y(other.y), initialY(other.initialY), W(other.W), H(other.H),
      textVertexArray(other.textVertexArray), cursorL(other.cursorL), cursorR(other.cursorR),
      step(other.step), ctrl(other.ctrl), shift(other.shift), infoBanner(other.infoBanner) {
    selectionHead = (other.selectionHead == &other.cursorL) ? &cursorL : &cursorR;
}

EditableText::EditableText(EditableText &&other) noexcept
    : Scene(std::move(other)), text(std::move(other.text)), currentFilePath(std::move(other.currentFilePath)),
      x(other.x), y(other.y), initialY(other.initialY), W(other.W), H(other.H),
      textVertexArray(std::move(other.textVertexArray)), cursorL(other.cursorL), cursorR(other.cursorR),
      step(other.step), ctrl(other.ctrl), shift(other.shift), infoBanner(std::move(other.infoBanner)) {
    selectionHead = (other.selectionHead == &other.cursorL) ? &cursorL : &cursorR;
    other.selectionHead = &other.cursorR;
}

EditableText &EditableText::operator=(const EditableText &other) {
    if (this == &other) {
        return *this;
    }
    Scene::operator=(other);
    text = other.text;
    currentFilePath = other.currentFilePath;
    x = other.x;
    y = other.y;
    initialY = other.initialY;
    W = other.W;
    H = other.H;
    textVertexArray = other.textVertexArray;
    cursorL = other.cursorL;
    cursorR = other.cursorR;
    step = other.step;
    ctrl = other.ctrl;
    shift = other.shift;
    infoBanner = other.infoBanner;
    selectionHead = (other.selectionHead == &other.cursorL) ? &cursorL : &cursorR;
    return *this;
}

EditableText &EditableText::operator=(EditableText &&other) noexcept {
    if (this == &other) {
        return *this;
    }
    Scene::operator=(std::move(other));
    text = std::move(other.text);
    currentFilePath = std::move(other.currentFilePath);
    x = other.x;
    y = other.y;
    initialY = other.initialY;
    W = other.W;
    H = other.H;
    textVertexArray = std::move(other.textVertexArray);
    cursorL = other.cursorL;
    cursorR = other.cursorR;
    step = other.step;
    ctrl = other.ctrl;
    shift = other.shift;
    infoBanner = std::move(other.infoBanner);
    selectionHead = (other.selectionHead == &other.cursorL) ? &cursorL : &cursorR;
    other.selectionHead = &other.cursorR;
    return *this;
}

std::ostream &operator<<(std::ostream &os, const EditableText &obj) {
    return obj.print(os);
}

EditableText::EditableText(sf::RenderWindow &window_, std::string initialText_, std::filesystem::path filePath,
                           const int16_t x_, const int16_t y_, const size_t W_, const size_t H_,
                           const SceneID sceneToReturnAt_)
    : Scene(window_, sceneToReturnAt_), text(std::move(initialText_)), currentFilePath(std::move(filePath)), x(x_),
      y(y_), W(W_), H(H_) {
    setupInfoBanner();
    updateVertexArray();
}
