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
#include <sstream>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <exception>
#include <stdexcept>
#include "ext/portable-file-dialogs.h"

class AppError : public std::runtime_error {
public:
    explicit AppError(const std::string &msg) : std::runtime_error(msg) {}

    friend std::ostream &operator<<(std::ostream &os, const AppError &err) {
        os << "AppError: " << err.what();
        return os;
    }
};

class ConfigurationError : public AppError {
public:
    ConfigurationError() : AppError("Error: Configuration file is malformed or missing.") {}

    friend std::ostream &operator<<(std::ostream &os, const ConfigurationError &err) {
        os << "ConfigurationError: " << err.what();
        return os;
    }
};

class AssetLoadError : public AppError {
public:
    AssetLoadError() : AppError("Error: Required assets could not be loaded.") {}

    friend std::ostream &operator<<(std::ostream &os, const AssetLoadError &err) {
        os << "AssetLoadError: " << err.what();
        return os;
    }
};

class SceneInitializationError : public AppError {
public:
    SceneInitializationError() : AppError("Error: Failed to initialize a required scene.") {}

    friend std::ostream &operator<<(std::ostream &os, const SceneInitializationError &err) {
        os << "SceneInitializationError: " << err.what();
        return os;
    }
};

class FontManager {
    uint8_t ascii[8 * 95]{};

    FontManager() {
        const std::filesystem::path sourcePath = SOURCE_DIR;
        const std::filesystem::path fontPath = sourcePath / "assets" / "font.txt";

        std::ifstream input(fontPath);
        if (!input.is_open()) {
            throw AssetLoadError();
        }
        int x;
        for (unsigned char &i: ascii) {
            input >> x;
            i = x;
        }
        input.close();
    }

public:
    friend std::ostream &operator<<(std::ostream &os, const FontManager &) {
        os << "FontManager (Singleton Instance holding custom ASCII definitions)";
        return os;
    }

    static const FontManager &getInstance() {
        static FontManager instance;
        return instance;
    }

    [[nodiscard]] uint8_t getByte(const size_t index) const {
        return ascii[index];
    }
};

namespace VertexArrayUtility {
    void insertRectangle(sf::VertexArray &v, const int16_t x, const int16_t y, const int16_t w,
                         const int16_t h, const sf::Color color) {
        v.append({{static_cast<float>(x), static_cast<float>(y)}, color, {0.0f, 0.0f}});
        v.append({{static_cast<float>(x + w), static_cast<float>(y)}, color, {0.0f, 0.0f}});
        v.append({{static_cast<float>(x), static_cast<float>(y + h)}, color, {0.0f, 0.0f}});
        v.append({{static_cast<float>(x + w), static_cast<float>(y)}, color, {0.0f, 0.0f}});
        v.append({{static_cast<float>(x), static_cast<float>(y + h)}, color, {0.0f, 0.0f}});
        v.append({{static_cast<float>(x + w), static_cast<float>(y + h)}, color, {0.0f, 0.0f}});
    }

    void moveAt(sf::VertexArray &v, const int16_t newX, const int16_t newY) {
        if (const size_t len = v.getVertexCount()) {
            const float stepX = static_cast<float>(newX) - v[0].position.x;
            const float stepY = static_cast<float>(newY) - v[0].position.y;
            for (size_t i = 0; i < len; i++) {
                v[i].position.x += stepX;
                v[i].position.y += stepY;
            }
        }
    }

    void insertChar(sf::VertexArray &v, const char c, const int16_t x, const int16_t y, const int16_t pixelSize,
                    const sf::Color color) {
        v.setPrimitiveType(sf::PrimitiveType::Triangles);

        if (c != ' ') {
            uint16_t offset;
            if (c < '!' || c > '~') {
                offset = (95 - 1) * 8;
            } else {
                offset = (c - '!') * 8;
            }
            for (uint16_t i = 0; i < 8; i++) {
                uint8_t line = FontManager::getInstance().getByte(offset + i);
                for (int16_t j = 0; j < 8; j++) {
                    const uint8_t _ = line & 1;
                    line >>= 1;
                    if (_) {
                        insertRectangle(v, static_cast<int16_t>(x + pixelSize * (7 - j)),
                                        static_cast<int16_t>(y + pixelSize * i), pixelSize,
                                        pixelSize, color);
                    }
                }
            }
        }
    }
}

class Settings {
    int16_t windowW = 800;
    int16_t windowH = 700;
    int16_t pixelSize = 2;

    sf::Color backgroundColor = sf::Color(255, 255, 255);
    sf::Color textColor = sf::Color(0, 0, 0);
    sf::Color cursorColor = sf::Color(0, 255, 0);

    bool isEnglish = true;

    void loadSettings(const bool resetDefaults = false) {
        const std::filesystem::path sourcePath = SOURCE_DIR;
        const std::filesystem::path settingsFile = sourcePath / "config" / "settings.txt";

        std::ifstream input(settingsFile);

        if (!input.is_open() && !resetDefaults) {
            throw ConfigurationError();
        }
        if (resetDefaults) {
            std::cout << "Default settings will be restored";
            windowW = 800;
            windowH = 700;
            pixelSize = 2;
            backgroundColor = sf::Color(255, 255, 255);
            textColor = sf::Color(0, 0, 0);
            cursorColor = sf::Color(0, 255, 0);
            isEnglish = true;
            return;
        }

        int r, g, b, x;
        input >> x;
        windowW = static_cast<int16_t>(x);
        input >> x;
        windowH = static_cast<int16_t>(x);
        input >> x;
        pixelSize = static_cast<int16_t>(x);

        input >> r >> g >> b;
        backgroundColor.r = r;
        backgroundColor.g = g;
        backgroundColor.b = b;
        input >> r >> g >> b;
        textColor.r = r;
        textColor.g = g;
        textColor.b = b;
        input >> r >> g >> b;
        cursorColor.r = r;
        cursorColor.g = g;
        cursorColor.b = b;

        if (std::string lang; input >> lang) {
            isEnglish = (lang == "eng");
        } else {
            isEnglish = true;
        }
        input.close();
    }

    void storeSettings() const {
        const std::filesystem::path sourcePath = SOURCE_DIR;
        const std::filesystem::path settingsFile = sourcePath / "config" / "settings.txt";

        std::ofstream output(settingsFile);
        output << static_cast<int>(windowW) << " " << static_cast<int>(windowH) << "\n" << pixelSize
                << "\n" << static_cast<int>(backgroundColor.r) << " " << static_cast<int>(backgroundColor.g) << " " <<
                static_cast<int>(backgroundColor.b)
                << "\n" << static_cast<int>(textColor.r) << " " << static_cast<int>(textColor.g) << " " << static_cast<
                    int>(textColor.b)
                << "\n" << static_cast<int>(cursorColor.r) << " " << static_cast<int>(cursorColor.g) << " " <<
                static_cast<int>(cursorColor.b)
                << "\n" << (isEnglish ? "eng" : "ro");
        output.close();
    }

public:
    friend std::ostream &operator<<(std::ostream &os, const Settings &obj) {
        os << "Settings -> windowW: " << obj.windowW << " windowH: " << obj.windowH
                << " pixelSize: " << obj.pixelSize << " backgroundColor: " << obj.backgroundColor.toInteger()
                << " textColor: " << obj.textColor.toInteger() << " cursorColor: " << obj.cursorColor.toInteger();
        return os;
    }

    Settings() { loadSettings(); }

    Settings(const Settings &) = delete;

    Settings &operator=(const Settings &) = delete;

    static Settings &getInstance() {
        static Settings instance;
        return instance;
    }

    virtual ~Settings() {
        storeSettings();
    }

    void update(const bool resetDefaults = false) {
        if (resetDefaults) {
            loadSettings(resetDefaults);
            storeSettings();
        } else {
            storeSettings();
            loadSettings();
        }
    }

    [[nodiscard]] bool getIsEnglish() const {
        return isEnglish;
    }
    void toggleLanguage() {
        isEnglish = !isEnglish;
    }
    [[nodiscard]] int16_t window_w() const {
        return windowW;
    }
    [[nodiscard]] int16_t window_h() const {
        return windowH;
    }
    [[nodiscard]] int16_t pixel_size() const {
        return pixelSize;
    }
    [[nodiscard]] sf::Color background_color() const {
        return backgroundColor;
    }
    [[nodiscard]] sf::Color text_color() const {
        return textColor;
    }
    [[nodiscard]] sf::Color cursor_color() const {
        return cursorColor;
    }

    void increase_pixel_size() {
        if (pixelSize + 1 < 15) {
            pixelSize = static_cast<int16_t>(pixelSize + 1);
        }
    }
    void decrease_pixel_size() {
        if (pixelSize - 1 >= 1) {
            pixelSize = static_cast<int16_t>(pixelSize - 1);
        }
    }
    void set_background_color(const sf::Color &background_color) {
        backgroundColor = background_color;
    }
    void set_text_color(const sf::Color &text_color) {
        textColor = text_color;
    }
    void set_cursor_color(const sf::Color &cursor_color) {
        cursorColor = cursor_color;
    }
};

enum class SceneID {
    MainMenu = 0,
    SettingsMenu = 1,
    BackgroundSettings = 2,
    TextSettings = 3,
    CursorSettings = 4,
    PixelSizeSettings = 5,
    RestoreDefaults = 6,
    Increase = 7,
    Decrease = 8,
    NewFile = 9,
    OpenFromDisk = 10,
    AIMode = 11,
    TextEditor = 12,
    Exit = 13,
    Intro = 14,
    ToggleLanguage = 15
};

class Scene {
    virtual void manageEvent() = 0;

    virtual void reset() = 0;

    bool requestExit = false;
    SceneID sceneToReturnAt = SceneID::Exit;
    SceneID defaultSceneToReturnAt = SceneID::Exit;

protected:
    sf::RenderWindow &window;
    std::optional<sf::Event> event;

    Scene(const Scene& other) = default;
    Scene(Scene&& other) noexcept
        : requestExit(other.requestExit), sceneToReturnAt(other.sceneToReturnAt),
          defaultSceneToReturnAt(other.defaultSceneToReturnAt), window(other.window), event(other.event) {}
    Scene& operator=(const Scene& other) {
        if (this == &other) return *this;
        requestExit = other.requestExit;
        sceneToReturnAt = other.sceneToReturnAt;
        defaultSceneToReturnAt = other.defaultSceneToReturnAt;
        event = other.event;
        return *this;
    }

    Scene& operator=(Scene&& other) noexcept {
        if (this == &other) return *this;
        requestExit = other.requestExit;
        sceneToReturnAt = other.sceneToReturnAt;
        defaultSceneToReturnAt = other.defaultSceneToReturnAt;
        event = other.event;
        return *this;
    }

public:
    virtual ~Scene() = default;

    friend std::ostream &operator<<(std::ostream &os, const Scene &obj) {
        return obj.print(os);
    }

    virtual std::ostream &print(std::ostream &os) const {
        os << "Base Scene Data -> window pointer: " << &window << ", requestExit status: " << (
            requestExit ? "true" : "false");
        return os;
    }

    explicit Scene(sf::RenderWindow &w, const SceneID sceneToReturnAt_)
        : sceneToReturnAt(sceneToReturnAt_), defaultSceneToReturnAt(sceneToReturnAt_), window(w) {
    }

    [[nodiscard]] virtual std::unique_ptr<Scene> clone() const = 0;

    virtual void draw() = 0;

    virtual void begin() = 0;

    virtual void resume() {
        requestExit = false;
    }

    void end() {
        requestExit = false;
        sceneToReturnAt = defaultSceneToReturnAt;
        reset();
    }

    void manageEvent(const sf::Event &e) {
        event = e;
        manageEvent();
    }

    void setSceneToReturnAt(const SceneID id) {
        sceneToReturnAt = id;
    }
    [[nodiscard]] SceneID getSceneToReturnAt() const {
        return sceneToReturnAt;
    }
    [[nodiscard]] bool hasFinished() const {
        return requestExit;
    }
    void exit() { requestExit = true; }
};

class Button : public Scene {
    std::function<void()> action;

    void reset() override {}

    void manageEvent() override {}

public:
    [[nodiscard]] std::unique_ptr<Scene> clone() const override {
        return std::make_unique<Button>(*this);
    }

    friend std::ostream &operator<<(std::ostream &os, const Button &obj) {
        return obj.print(os);
    }

    std::ostream &print(std::ostream &os) const override {
        Scene::print(os);
        os << " | Derived Button -> (System functional trigger tile)";
        return os;
    }

    Button(sf::RenderWindow &window, std::function<void()> func, const SceneID sceneToReturnAt_)
        : Scene(window, sceneToReturnAt_), action(std::move(func)) {}

    void draw() override {}

    void begin() override {
        action();
        exit();
    }
};

class ReadOnlyText {
public:
    friend std::ostream &operator<<(std::ostream &os, const ReadOnlyText &obj) {
        os << "ReadOnlyText component -> text content: \"" << obj.text << "\" pos: (" << obj.x << "," << obj.y << ")";
        return os;
    }

private:
    sf::RenderTarget *target;
    std::string text;
    int16_t x;
    int16_t y;
    int16_t pixelSize;
    size_t W;
    size_t H;
    sf::Color textColor;
    bool lineWrappable;
    sf::VertexArray textVertexArray;

    void updateVertexArray() {
        textVertexArray.clear();
        textVertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);

        std::string formattedText = text;
        const auto charPoz = static_cast<int16_t>(9 * pixelSize);
        const auto rightLimit = static_cast<int16_t>(W + x - charPoz);
        const size_t rowLen = (W + x) / charPoz;

        size_t i = 0;
        if (lineWrappable) {
            while (i < formattedText.size()) {
                if (formattedText[i] == '\t') {
                    formattedText[i] = ' ';
                    formattedText.insert(i, "   ");
                    i += 4;
                } else if (formattedText[i] == '\n') {
                    formattedText[i] = ' ';
                    i++;
                    while (i % rowLen) {
                        formattedText.insert(i, " ");
                        i++;
                    }
                } else { i++; }
            }
            i = 0;
            const size_t size = formattedText.size();
            for (int16_t r = y; i < size; r = static_cast<int16_t>(r + charPoz)) {
                for (int16_t c = x; c <= rightLimit && i < size; c = static_cast<int16_t>(c + charPoz)) {
                    VertexArrayUtility::insertChar(textVertexArray, formattedText[i], c, r, pixelSize, textColor);
                    i++;
                }
            }
        } else {
            while (i < formattedText.size()) {
                if (formattedText[i] == '\t') {
                    formattedText[i] = ' ';
                    formattedText.insert(i, "   ");
                    i += 4;
                } else {
                    i++;
                }
            }
            i = 0;
            const size_t size = formattedText.size();
            int16_t r = y;
            while (i < size) {
                int16_t c = x;
                while (i < size && formattedText[i] != '\n') {
                    VertexArrayUtility::insertChar(textVertexArray, formattedText[i], c, r, pixelSize, textColor);
                    i++;
                    c = static_cast<int16_t>(c + charPoz);
                }
                if (i < size && formattedText[i] == '\n') {
                    i++;
                }
                r = static_cast<int16_t>(r + charPoz);
            }
        }
    }

public:
    ReadOnlyText() : target(nullptr), x(0), y(0), pixelSize(5), W(0), H(0), lineWrappable(true) {}

    explicit ReadOnlyText(sf::RenderTarget *target_, std::string text_, const int16_t x_, const int16_t y_,
                          const uint8_t pixelSize_, const size_t W_, const size_t H_, const sf::Color color_,
                          const bool lineWrappable_ = true)
        : target(target_), text(std::move(text_)), x(x_), y(y_), pixelSize(pixelSize_), W(W_), H(H_),
          textColor(color_), lineWrappable(lineWrappable_) {
        updateVertexArray();
    }

    void setTextColor(const sf::Color &color) {
        textColor = color;
    }
    void setPixelSize(const uint8_t &pixel_size) {
        pixelSize = pixel_size;
    }
    void draw() const {
        target->draw(textVertexArray);
    }
    void refresh() {
        updateVertexArray();
    }

    void manageResizedEvent() {
        if (lineWrappable) {
            W = target->getSize().x;
            H = target->getSize().y;
            updateVertexArray();
        }
    }

    void moveAt(const int16_t newX, const int16_t newY) {
        const auto stepX = static_cast<int16_t>(newX - x);
        const auto stepY = static_cast<int16_t>(newY - y);
        const size_t count = textVertexArray.getVertexCount();
        for (size_t i = 0; i < count; i++) {
            textVertexArray[i].position.x += static_cast<float>(stepX);
            textVertexArray[i].position.y += static_cast<float>(stepY);
        }
        x = newX;
        y = newY;
    }
};

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

    void updateVertexArray() {
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

        if (formattedCursorR == formattedText.size()) formattedText += " ";

        size_t visibleTextL = 0;
        size_t visibleTextR = formattedText.size();
        const size_t maxChars = (W * H) / (charPoz * charPoz);
        size_t focusCursor = (selectionHead == &cursorL) ? formattedCursorL : formattedCursorR;

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

    void deleteSelectedText() {
        if (cursorL != cursorR) {
            text.erase(cursorL, cursorR - cursorL + 1);
            cursorR = cursorL;
        }
    }

    void manageKey(const sf::Keyboard::Scancode k) {
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

    void reset() override {}

    void manageEvent() override {
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
            } else if ((c >= 32 && c < 127) || c == '\n' || c == '\t') {
                deleteSelectedText();
                text.insert(cursorR++, 1, c);
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

    void setupInfoBanner() {
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

public:
    EditableText(const EditableText &other)
        : Scene(other), text(other.text), currentFilePath(other.currentFilePath),
          x(other.x), y(other.y), initialY(other.initialY), W(other.W), H(other.H),
          textVertexArray(other.textVertexArray), cursorL(other.cursorL), cursorR(other.cursorR),
          step(other.step), ctrl(other.ctrl), shift(other.shift), infoBanner(other.infoBanner) {
        selectionHead = (other.selectionHead == &other.cursorL) ? &cursorL : &cursorR;
    }

    EditableText(EditableText &&other) noexcept
        : Scene(std::move(other)), text(std::move(other.text)), currentFilePath(std::move(other.currentFilePath)),
          x(other.x), y(other.y), initialY(other.initialY), W(other.W), H(other.H),
          textVertexArray(std::move(other.textVertexArray)), cursorL(other.cursorL), cursorR(other.cursorR),
          step(other.step), ctrl(other.ctrl), shift(other.shift), infoBanner(std::move(other.infoBanner)) {
        selectionHead = (other.selectionHead == &other.cursorL) ? &cursorL : &cursorR;
        other.selectionHead = &other.cursorR;
    }

    EditableText &operator=(const EditableText &other) {
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

    EditableText &operator=(EditableText &&other) noexcept {
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

    ~EditableText() override = default;

    [[nodiscard]] std::filesystem::path getFilePath() const {
        return currentFilePath;
    }
    [[nodiscard]] std::string getText() const {
        return text;
    }
    [[nodiscard]] std::unique_ptr<Scene> clone() const override {
        return std::make_unique<EditableText>(*this);
    }

    friend std::ostream &operator<<(std::ostream &os, const EditableText &obj) {
        return obj.print(os);
    }

    std::ostream &print(std::ostream &os) const override {
        Scene::print(os);
        os << " | Derived EditableText -> filePath: " << currentFilePath.string() << ", Total chars: " << text.size();
        return os;
    }

    explicit EditableText(sf::RenderWindow &window_, std::string initialText_, std::filesystem::path filePath,
                          const int16_t x_, const int16_t y_, const size_t W_, const size_t H_,
                          const SceneID sceneToReturnAt_)
        : Scene(window_, sceneToReturnAt_), text(std::move(initialText_)), currentFilePath(std::move(filePath)), x(x_),
          y(y_), W(W_), H(H_) {
        setupInfoBanner();
        updateVertexArray();
    }

    void draw() override {
        infoBanner.draw();
        window.draw(textVertexArray);
    }

    void begin() override {
        setupInfoBanner();
        updateVertexArray();
    }

    void resume() override {
        Scene::resume();
        ctrl = false;
        shift = false;
    }
};

class Greet : public Scene {
    sf::Vector2i formerPos;
    unsigned int formerWindowW = 0;
    unsigned int formerWindowH = 0;
    uint8_t pixelWidth = 3;
    const size_t W = pixelWidth * 9 * 8 * 3;
    const size_t H = pixelWidth * 9 * 8;
    size_t textSize = 0;
    ReadOnlyText matrix;
    ReadOnlyText greeting;
    int16_t x = 0;

    void setProperties() {
        formerWindowW = window.getSize().x;
        formerWindowH = window.getSize().y;
        formerPos = window.getPosition();

        sf::Vector2i desiredPos;
        desiredPos.x = static_cast<int>(formerPos.x + (window.getSize().x - W) / 2);
        desiredPos.y = static_cast<int>(formerPos.y + (window.getSize().y - H) / 2);
        window.create(sf::VideoMode({static_cast<unsigned int>(W), static_cast<unsigned int>(H)}), "", sf::Style::None,
                       sf::State::Windowed);
        window.setPosition(desiredPos);
        window.setFramerateLimit(30);

    }

    void resetProperties() {
        window.create(sf::VideoMode({formerWindowW, formerWindowH}), "car_plus_plus", sf::Style::Default,
                       sf::State::Windowed);
        window.setFramerateLimit(10);
    }

    void reset() override {
        x = static_cast<int16_t>(W);
        greeting.moveAt(x, 0);
        resetProperties();
    }

    void manageEvent() override {}

public:
    [[nodiscard]] std::unique_ptr<Scene> clone() const override { return std::make_unique<Greet>(*this); }

    friend std::ostream &operator<<(std::ostream &os, const Greet &obj) {
        return obj.print(os);
    }

    std::ostream &print(std::ostream &os) const override {
        Scene::print(os);
        os << " | Derived Greet Panel -> W: " << W << ", H: " << H << ", textSize: " << textSize;
        return os;
    }

    explicit Greet(sf::RenderWindow &window_, const std::string &s, const SceneID sceneToReturnAt_)
        : Scene(window_, sceneToReturnAt_) {
        textSize = s.size();
        std::string backgroundText;
        constexpr uint8_t backgroundTextLen = 8 * 8 * 3;
        uint8_t i = 0;
        while (i < backgroundTextLen) {
            for (char c = ' '; i < backgroundTextLen && c <= '~'; c++) {
                backgroundText += c;
                i++;
            }
        }
        matrix = ReadOnlyText{&window, backgroundText, 0, 0, pixelWidth, W, H, Settings::getInstance().text_color()};
        greeting = ReadOnlyText{
            &window, s, static_cast<int16_t>(W), 0, static_cast<uint8_t>(pixelWidth * 9),
            9 * 9 * pixelWidth * textSize, static_cast<size_t>(9 * 8 * pixelWidth),
            Settings::getInstance().text_color(), false
        };
        x = static_cast<int16_t>(W);
    }

    void draw() override {
        if (x < -static_cast<int16_t>(9 * 8 * pixelWidth * textSize)) {
            resetProperties();
            exit();
        }
        matrix.draw();
        greeting.draw();
        x = static_cast<int16_t>(x - 9 * pixelWidth);
        greeting.moveAt(x, 0);
    }

    void begin() override {
        setProperties();
    }
};

class TilePanel : public Scene {
    std::string titleText;
    const uint8_t target;
    ReadOnlyText titleObject;

    int8_t cursorX = 0;
    int8_t cursorY = 0;
    const int16_t x = 0;
    int16_t y = static_cast<int16_t>(Settings::getInstance().pixel_size() * 9);
    int16_t cellW = 0;
    int16_t cellH = 0;
    int columns = 0;
    int rows = 0;
    std::vector<sf::Color> colors;
    sf::VertexArray tableVertexArray;
    sf::VertexArray cursorVertexArray;

    void createTilePanel() {
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

    void manageResizedEvent() {
        titleObject.manageResizedEvent();
    }

    void updateCursor() {
        VertexArrayUtility::moveAt(cursorVertexArray, static_cast<int16_t>(x + cellW * cursorX),
                                   static_cast<int16_t>(y + cellH * cursorY));
    }

    void reset() override {
        cursorX = 0;
        cursorY = 0;
    }

    void manageEvent() override {
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

public:
    [[nodiscard]] std::unique_ptr<Scene> clone() const override {
        return std::make_unique<TilePanel>(*this);
    }

    friend std::ostream &operator<<(std::ostream &os, const TilePanel &obj) {
        return obj.print(os);
    }

    std::ostream &print(std::ostream &os) const override {
        Scene::print(os);
        os << " | Derived TilePanel -> title: \"" << titleText << "\", Active grid structure: " << rows << "x" <<
                columns;
        return os;
    }

    TilePanel(sf::RenderWindow &window_, std::string title_, const uint8_t target_, const SceneID sceneToReturnAt_)
        : Scene(window_, sceneToReturnAt_), titleText(std::move(title_)), target(target_) {
        titleObject = ReadOnlyText{
            &window, titleText, 0, 0, static_cast<uint8_t>(Settings::getInstance().pixel_size()),
            static_cast<uint16_t>(Settings::getInstance().window_w()),
            static_cast<uint16_t>(Settings::getInstance().pixel_size() * 9), Settings::getInstance().text_color(), false
        };
        createTilePanel();
    }

    void begin() override {
        titleObject.setTextColor(Settings::getInstance().text_color());
        titleObject.setPixelSize(static_cast<uint8_t>(Settings::getInstance().pixel_size()));
        titleObject.refresh();
        createTilePanel();
    }

    void draw() override {
        titleObject.draw();
        window.draw(tableVertexArray);
        window.draw(cursorVertexArray);
    }
};

class Menu : public Scene {
    ReadOnlyText text;
    std::vector<SceneID> actions;
    int cursor = 0;
    sf::VertexArray cursorVertexArray;
    int len = 0;
    int16_t lineHeight = 0;
    std::string parameters;

    void updateCursor() {
        lineHeight = static_cast<int16_t>(Settings::getInstance().pixel_size() * 9);
        cursorVertexArray.clear();
        VertexArrayUtility::insertRectangle(cursorVertexArray, 0, static_cast<int16_t>((cursor + 1) * lineHeight),
                                            static_cast<int16_t>(window.getSize().x), lineHeight,
                                            Settings::getInstance().cursor_color());
    }

    void reset() override {}

    void manageEvent() override {
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

public:
    [[nodiscard]] std::unique_ptr<Scene> clone() const override {
        return std::make_unique<Menu>(*this);
    }

    friend std::ostream &operator<<(std::ostream &os, const Menu &obj) {
        return obj.print(os);
    }

    std::ostream &print(std::ostream &os) const override {
        Scene::print(os);
        os << " | Derived Menu -> Active components count: " << len << ", selected index: " << cursor;
        return os;
    }

    Menu(sf::RenderWindow &window_, const std::string &menuText, const std::vector<SceneID> &actions_,
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

    void draw() override {
        window.draw(cursorVertexArray);
        text.draw();
    }

    void begin() override {
        text.setTextColor(Settings::getInstance().text_color());
        text.setPixelSize(Settings::getInstance().pixel_size());
        text.refresh();
        updateCursor();
    }
};

class SceneManager {
    std::unordered_map<SceneID, std::unique_ptr<Scene> > scenes;
    sf::VertexArray background;
    Scene *currentScene = nullptr;

    sf::RenderWindow window;

    void updateBackground() {
        background.clear();
        VertexArrayUtility::insertRectangle(background, 0, 0, static_cast<int16_t>(window.getSize().x),
                                            static_cast<int16_t>(window.getSize().y),
                                            Settings::getInstance().background_color());
    }

    bool handleFileOperation(SceneID action) {
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

    [[nodiscard]] Scene *resolve(const SceneID id) {
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

    void createButtons() {
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
        for (auto &[id, config]: buttonRegistry) scenes[id] = std::make_unique<Button>(
                                                     window, config.first, config.second);
    }

    void createScenes() {
        bool eng = Settings::getInstance().getIsEnglish();
        std::string bgTitle = eng ? "Background: Enter to select" : "Fundal: Enter pentru selectie";
        std::string textTitle = eng ? "Text: Enter to select" : "Text: Enter pentru selectie";
        std::string cursorTitle = eng ? "Cursor: Enter to select" : "Cursor: Enter pentru selectie";
        std::string aiTitle = eng ? "malware successfully installed" : "malware instalat cu succes";

        scenes[SceneID::BackgroundSettings] = std::make_unique<TilePanel>(window, bgTitle, 0, SceneID::SettingsMenu);
        scenes[SceneID::TextSettings] = std::make_unique<TilePanel>(window, textTitle, 1, SceneID::SettingsMenu);
        scenes[SceneID::CursorSettings] = std::make_unique<TilePanel>(window, cursorTitle, 2, SceneID::SettingsMenu);
        scenes[SceneID::AIMode] = std::make_unique<Greet>(window, aiTitle, SceneID::Exit);

        const std::filesystem::path sourcePath = SOURCE_DIR;
        std::string filename = eng ? "menu_eng.txt" : "menu_ro.txt";
        std::ifstream menuFile(sourcePath / "config" / filename);
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
            }
            else if (key == "Return") {
                returnID = std::stoi(value);
            }
            else if (key == "Title") {
                menuText = value;
            }
            else if (key == "Item") {
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

public:
    friend std::ostream &operator<<(std::ostream &os, const SceneManager &obj) {
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
            }
            else os << "nullptr";
        }
        os << "\n=======================================================================";
        return os;
    }

    explicit SceneManager() {
        window.create(sf::VideoMode({static_cast<uint16_t>(Settings::getInstance().window_w()), static_cast<uint16_t>(Settings::getInstance().window_h())}), "car_plus_plus", sf::Style::Default, sf::State::Windowed);
        std::cout << "The window was created successfully\n";
        window.setFramerateLimit(10);

        background.setPrimitiveType(sf::PrimitiveType::Triangles);
        updateBackground();
        createButtons();
        createScenes();
        currentScene = scenes[SceneID::Intro].get();
    }

    void run() {
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
};

int main() {
    try {
        SceneManager sm{};

        std::cout << "Afisare Polymorphic Composition Output:\n" << sm << "\n";

        sm.run();
    } catch (const ConfigurationError &e) {
        std::cerr << "Config Error Catch Block: " << e << std::endl;
        return 1;
    } catch (const AssetLoadError &e) {
        std::cerr << "Asset Error Catch Block: " << e << std::endl;
        return 1;
    } catch (const std::exception &e) {
        std::cerr << "General Exception Block: " << e.what() << std::endl;
        return 1;
    }
    std::cout << "The program finished successfully\n";
    return 0;
}