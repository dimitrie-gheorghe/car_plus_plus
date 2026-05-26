#include <functional>
#include <filesystem>
#include <memory>
#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>
#include <utility>

class VertexArrayUtility {
    uint8_t ascii[8 * 95]{};

public:
    virtual ~VertexArrayUtility() = default;

    friend std::ostream &operator<<(std::ostream &os, const VertexArrayUtility &obj) {
        os << obj.ascii[0];
        return os;
    }

    VertexArrayUtility() {
        std::ifstream input("./assets/font.txt");
        int x;
        for (unsigned char &i: ascii) {
            input >> x;
            i = x;
        }
        input.close();
    }

    static void insertRectangle(sf::VertexArray &v, const int16_t x, const int16_t y, const int16_t w,
                                const int16_t h, const sf::Color color) {
        v.append({{static_cast<float>(x), static_cast<float>(y)}, color, {0.0f, 0.0f}});

        v.append({{static_cast<float>(x + w), static_cast<float>(y)}, color, {0.0f, 0.0f}});
        v.append({{static_cast<float>(x), static_cast<float>(y + h)}, color, {0.0f, 0.0f}});

        v.append({{static_cast<float>(x + w), static_cast<float>(y)}, color, {0.0f, 0.0f}});
        v.append({{static_cast<float>(x), static_cast<float>(y + h)}, color, {0.0f, 0.0f}});

        v.append({{static_cast<float>(x + w), static_cast<float>(y + h)}, color, {0.0f, 0.0f}});
    }

    static void moveAt(sf::VertexArray &v, const int16_t newX, const int16_t newY) {
        if (const size_t len = v.getVertexCount()) {
            const float stepX = static_cast<float>(newX) - v[0].position.x;
            const float stepY = static_cast<float>(newY) - v[0].position.y;
            for (size_t i = 0; i < len; i++) {
                v[i].position.x += stepX;
                v[i].position.y += stepY;
            }
        }
    }

    static void translateX(sf::VertexArray &v, const float step) {
        const size_t len = v.getVertexCount();
        for (size_t i = 0; i < len; i++) {
            v[i].position.x += step;
        }
    }

    static void translateY(sf::VertexArray &v, const float step) {
        const size_t len = v.getVertexCount();
        for (size_t i = 0; i < len; i++) {
            v[i].position.y += step;
        }
    }

    void insertChar(sf::VertexArray &v, const char c, const int16_t x, const int16_t y, const int16_t pixelSize,
                    const sf::Color color) const {
        v.setPrimitiveType(sf::PrimitiveType::Triangles);

        if (c != ' ') {
            uint16_t offset;
            if (c < '!' || c > '~') {
                offset = (95 - 1) * 8;
            } else {
                offset = (c - '!') * 8;
            }
            for (uint16_t i = 0; i < 8; i++) {
                uint8_t line = ascii[offset + i];
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
};

class Settings {
    int16_t windowW = 800;
    int16_t windowH = 700;
    int16_t pixelSize = 2;

    sf::Color backgroundColor = sf::Color(255, 255, 255);
    sf::Color textColor = sf::Color(0, 0, 0);
    sf::Color cursorColor = sf::Color(0, 255, 0);

    void loadSettings(const bool resetDefaults = false) {
        const std::filesystem::path sourcePath = SOURCE_DIR;
        const std::filesystem::path settingsFile = sourcePath / "config" / "settings.txt";

        std::ifstream input(settingsFile);

        if (!input.is_open() || resetDefaults) {
            if (resetDefaults) {
                std::cout << "Default settings will be restored";
            } else {
                std::cerr << "Could not open: " << settingsFile << "\nDefault settings will be used instead";
            }
            windowW = 800;
            windowH = 700;
            pixelSize = 2;
            backgroundColor = sf::Color(255, 255, 255);
            textColor = sf::Color(0, 0, 0);
            cursorColor = sf::Color(0, 255, 0);
            return;
        }

        int r, g, b, x;

        input >> x;
        windowW = static_cast<int16_t>(x); // necessary because the colors are uint16_t and they are not read correctly
        input >> x;
        windowH = static_cast<int16_t>(x);
        input >> x;
        pixelSize = static_cast<int16_t>(x);

        input >> r >> g >> b; // necessary because the colors are uint8_t and they are not read correctly
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

        input.close();
    }

    void storeSettings() const {
        // save the settings
        const std::filesystem::path sourcePath = SOURCE_DIR;
        const std::filesystem::path settingsFile = sourcePath / "config" / "settings.txt";

        std::ofstream output(settingsFile);

        output << static_cast<int>(windowW) << " " << static_cast<int>(windowH) <<
                "\n" << pixelSize
                << "\n" << static_cast<int>(backgroundColor.r)
                << " " << static_cast<int>(backgroundColor.g)
                << " " << static_cast<int>(backgroundColor.b)

                << "\n" << static_cast<int>(textColor.r)
                << " " << static_cast<int>(textColor.g)
                << " " << static_cast<int>(textColor.b)

                << "\n" << static_cast<int>(cursorColor.r)
                << " " << static_cast<int>(cursorColor.g)
                << " " << static_cast<int>(cursorColor.b);

        output.close();
    }

public:
    friend std::ostream &operator<<(std::ostream &os, const Settings &obj) {
        os << "windowW: " << obj.windowW
                << " windowH: " << obj.windowH
                << " pixelSize: " << obj.pixelSize
                << " backgroundColor: " << obj.backgroundColor.toInteger()
                << " textColor: " << obj.textColor.toInteger()
                << " cursorColor: " << obj.cursorColor.toInteger();
        return os;
    }

    Settings() {
        loadSettings();
    }

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
    MainMenu,
    SettingsMenu,
    BackgroundSettings,
    TextSettings,
    CursorSettings,
    PixelSizeSettings,
    RestoreDefaults,
    Increase,
    Decrease,
    NewFile,
    OpenFromDisk,
    AIMode,
    DemoTextEditor,
    Exit
};

class Scene {
protected:
    sf::RenderWindow *window = nullptr;
    std::optional<sf::Event> event;
    SceneID nextScene = SceneID::Exit;
    bool requestExit = false;

public:
    friend std::ostream &operator<<(std::ostream &os, const Scene &obj) {
        os << "These are the contents of the Scene class: "
                << "window: " << obj.window
                << " requestExit: " << obj.requestExit;
        return os;
    }

    explicit Scene(sf::RenderWindow *w) : window(w) {
    }

    virtual ~Scene() = default;

    virtual void manageEvent() = 0;

    virtual void update() {
    }

    virtual void draw() = 0;

    virtual void refresh() {
    }

    virtual void reset() {
        requestExit = false;
        nextScene = SceneID::Exit;
    }

    void manageEvent(const sf::Event &e) {
        event = e;
        manageEvent();
    }

    void setNextScene(const SceneID id) {
        nextScene = id;
    }

    [[nodiscard]] SceneID getNextScene() const {
        return nextScene;
    }

    [[nodiscard]] bool hasFinished() const {
        return requestExit;
    }

    void exit() {
        requestExit = true;
    }
};

class Button : public Scene {
    std::function<void()> action;
    SceneID sceneToReturnAt;

public:
    friend std::ostream &operator<<(std::ostream &os, const Button &obj) {
        os << "Buttons don't have much data... : " << static_cast<const Scene &>(obj);
        return os;
    }

    Button(sf::RenderWindow *window, std::function<void()> func, const SceneID returnScene)
        : Scene(window), action(std::move(func)), sceneToReturnAt(returnScene) {
    }

    void manageEvent() override {
    }

    void update() override {
        action();
        setNextScene(sceneToReturnAt);
        exit();
    }

    void draw() override {
    }
};

class ReadOnlyText : VertexArrayUtility {
public:
    friend std::ostream &operator<<(std::ostream &os, const ReadOnlyText &obj) {
        os << "This is the ReadOnlyText class"
                << static_cast<const VertexArrayUtility &>(obj)
                << " target: " << obj.target
                << " text: " << obj.text
                << " x: " << obj.x
                << " y: " << obj.y
                << " pixelSize: " << obj.pixelSize
                << " W: " << obj.W
                << " H: " << obj.H
                << " textColor: " << obj.textColor.toInteger()
                << " backgroundColor: " << obj.backgroundColor.toInteger()
                << " lineWrappable: " << obj.lineWrappable
                << " backgroundVertexArray: " << obj.backgroundVertexArray.getVertexCount()
                << " textVertexArray: " << obj.textVertexArray.getVertexCount();
        return os;
    }

    ReadOnlyText(const ReadOnlyText &other) = default;

    ReadOnlyText(ReadOnlyText &&other) noexcept
        : VertexArrayUtility(std::move(other)),
          target(other.target),
          text(std::move(other.text)),
          x(other.x),
          y(other.y),
          pixelSize(other.pixelSize),
          W(other.W),
          H(other.H),
          textColor(other.textColor),
          backgroundColor(other.backgroundColor),
          lineWrappable(other.lineWrappable),
          backgroundVertexArray(std::move(other.backgroundVertexArray)),
          textVertexArray(std::move(other.textVertexArray)) {
    }

    ReadOnlyText &operator=(const ReadOnlyText &other) {
        if (this == &other)
            return *this;
        VertexArrayUtility::operator =(other);
        target = other.target;
        text = other.text;
        x = other.x;
        y = other.y;
        pixelSize = other.pixelSize;
        W = other.W;
        H = other.H;
        textColor = other.textColor;
        backgroundColor = other.backgroundColor;
        lineWrappable = other.lineWrappable;
        backgroundVertexArray = other.backgroundVertexArray;
        textVertexArray = other.textVertexArray;
        return *this;
    }

    ReadOnlyText &operator=(ReadOnlyText &&other) noexcept {
        if (this == &other)
            return *this;
        VertexArrayUtility::operator =(std::move(other));
        target = other.target;
        text = std::move(other.text);
        x = other.x;
        y = other.y;
        pixelSize = other.pixelSize;
        W = other.W;
        H = other.H;
        textColor = other.textColor;
        backgroundColor = other.backgroundColor;
        lineWrappable = other.lineWrappable;
        backgroundVertexArray = std::move(other.backgroundVertexArray);
        textVertexArray = std::move(other.textVertexArray);
        return *this;
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
    sf::Color backgroundColor;
    bool lineWrappable;

    sf::VertexArray backgroundVertexArray;
    sf::VertexArray textVertexArray;

    void updateVertexArray() {
        textVertexArray.clear();
        textVertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);

        backgroundVertexArray.clear();
        backgroundVertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);

        insertRectangle(
            backgroundVertexArray,
            x, y,
            static_cast<int16_t>(W),
            static_cast<int16_t>(H),
            backgroundColor
        );

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
                } else {
                    i++;
                }
            }

            i = 0;
            const size_t size = formattedText.size();

            for (int16_t r = y; i < size; r = static_cast<int16_t>(r + charPoz)) {
                for (int16_t c = x; c <= rightLimit && i < size; c = static_cast<int16_t>(c + charPoz)) {
                    insertChar(textVertexArray, formattedText[i], c, r, pixelSize, textColor);
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
                    insertChar(textVertexArray, formattedText[i], c, r, pixelSize, textColor);
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
    ReadOnlyText() : lineWrappable(true) {
        target = nullptr;
        x = 0;
        y = 0;
        pixelSize = 5;
        W = 0;
        H = 0;
    };

    explicit ReadOnlyText(sf::RenderTarget *target_, std::string text_, const int16_t x_, const int16_t y_,
                          const uint8_t pixelSize_,
                          const size_t W_, const size_t H_, const sf::Color color_,
                          const sf::Color background_, const bool lineWrappable_ = true) : target(target_),
        text(std::move(text_)),
        x(x_), y(y_),
        pixelSize(pixelSize_), W(W_), H(H_),
        textColor(color_),
        backgroundColor(background_), lineWrappable(lineWrappable_) {
        updateVertexArray();
    }


    void setTextColor(const sf::Color &color) {
        textColor = color;
    }

    void setBackgroundColor(const sf::Color &color) {
        backgroundColor = color;
    }

    void setPixelSize(const uint8_t &pixel_size) {
        pixelSize = pixel_size;
    }

    void draw() const {
        target->draw(backgroundVertexArray);
        target->draw(textVertexArray);
    }

    void refresh() {
        updateVertexArray();
    }

    void manageResizedEvent() {
        //reflow the text
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

class EditableText : public Scene, VertexArrayUtility {
    std::string text;
    int16_t x = 0;
    int16_t y = 0;
    size_t W = 0;
    size_t H = 0;

    sf::VertexArray backgroundVertexArray;
    sf::VertexArray textVertexArray;

    size_t cursorL = 0;
    size_t cursorR = 0;
    size_t *selectionHead = &cursorR;
    size_t step = 0;

    bool ctrl = false;
    bool shift = false;

    void updateVertexArray() {
        W = window->getSize().x;
        H = window->getSize().x;

        textVertexArray.clear();
        textVertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);

        backgroundVertexArray.clear();
        backgroundVertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);

        insertRectangle(backgroundVertexArray, x, y, static_cast<int16_t>(W),
                        static_cast<int16_t>(H), Settings::getInstance().background_color());

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
                if (old_i < formattedCursorL) formattedCursorL += i - old_i - 1;
                if (old_i < formattedCursorR) formattedCursorR += i - old_i - 1;
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
        size_t focusCursor = (selectionHead == &cursorL) ? formattedCursorL : formattedCursorR;

        while (focusCursor - visibleTextL + step >= maxChars) {
            visibleTextL += step;
        }
        if (visibleTextL + maxChars < visibleTextR) {
            visibleTextR = visibleTextL + maxChars;
        }

        for (size_t r = y; visibleTextL < visibleTextR; r += charPoz) {
            for (size_t c = x; c <= rightLimit && visibleTextL < visibleTextR; c += charPoz) {
                if (formattedCursorL <= visibleTextL && visibleTextL <= formattedCursorR) {
                    insertRectangle(textVertexArray, static_cast<int16_t>(c),
                                    static_cast<int16_t>(r),
                                    static_cast<int16_t>(Settings::getInstance().pixel_size() * 9),
                                    static_cast<int16_t>(Settings::getInstance().pixel_size() * 9),
                                    Settings::getInstance().cursor_color());
                }
                if ('!' <= formattedText[visibleTextL] && formattedText[visibleTextL] <= '~') {
                    insertChar(textVertexArray, formattedText[visibleTextL],
                               static_cast<int16_t>(c), static_cast<int16_t>(r), Settings::getInstance().pixel_size(),
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
        size_t newCursor;
        if (selectionHead == &cursorR) {
            newCursor = cursorR;
        } else {
            newCursor = cursorL;
        }
        if (k == sf::Keyboard::Scancode::Right) {
            if (*selectionHead + 1 <= text.size()) {
                newCursor++;
            }
        } else if (k == sf::Keyboard::Scancode::Left) {
            if (static_cast<int16_t>(*selectionHead - 1) >= 0) {
                newCursor--;
            }
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
            // It is very important to allow negative values (actually -1)
            for (uint8_t i = 0; i < 2; i++) {
                if (tmpCursor > -1) {
                    tmpCursor--;
                }
                while (tmpCursor > -1 && text[tmpCursor] != '\n') {
                    tmpCursor--;
                }
            }
            if (tmpCursor < static_cast<int>(text.size()) && l) {
                tmpCursor++;
                l--;
            }
            while (tmpCursor < static_cast<int>(text.size()) && text[tmpCursor] != '\n' && l) {
                tmpCursor++;
                l--;
            }
            if (tmpCursor + 1 == static_cast<int>(former_l) && tmpCursor == static_cast<int>(newCursor)) {
                newCursor = 0;
            } else {
                newCursor = static_cast<size_t>(tmpCursor);
            }
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
                if (newCursor < text.size()) {
                    (newCursor)++;
                }
                while (newCursor < text.size() && text[newCursor] != '\n') {
                    (newCursor)++;
                }
            }
            if (newCursor < text.size() && l) {
                (newCursor)++;
                l--;
            }
            while (newCursor < text.size() && text[newCursor] != '\n' && l) {
                (newCursor)++;
                l--;
            }
        }
        if (newCursor != *selectionHead) {
            if (shift) {
                if (selectionHead == &cursorL) {
                    cursorL = newCursor;
                } else {
                    cursorR = newCursor;
                }
                if (cursorL > cursorR) {
                    size_t _ = cursorL;
                    cursorL = cursorR;
                    cursorR = _;
                    if (selectionHead == &cursorL) {
                        selectionHead = &cursorR;
                    } else {
                        selectionHead = &cursorL;
                    }
                }
            } else {
                if (cursorL == cursorR) {
                    cursorL = newCursor;
                    cursorR = newCursor;
                } else {
                    if (k == sf::Keyboard::Scancode::Left || k == sf::Keyboard::Scancode::Up) {
                        cursorR = cursorL;
                    } else {
                        cursorL = cursorR;
                    }
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
                // cut
                if (cursorL != cursorR) {
                    sf::Clipboard::setString(text.substr(cursorL, cursorR - cursorL + 1));
                }
                deleteSelectedText();

                updateRequired = true;
            } else if (k == sf::Keyboard::Scancode::C) {
                // copy
                if (cursorL != cursorR) {
                    sf::Clipboard::setString(text.substr(cursorL, cursorR - cursorL + 1));
                }
            } else if (k == sf::Keyboard::Scancode::V) {
                // paste
                const std::string s = sf::Clipboard::getString();
                if (!s.empty()) {
                    deleteSelectedText();
                    text.insert(cursorL, s);
                    cursorL += s.size();
                    cursorR = cursorL;

                    updateRequired = true;
                }
            } else if (k == sf::Keyboard::Scancode::A) {
                // select all
                cursorL = 0;
                cursorR = text.size();

                updateRequired = true;
            } else if (k == sf::Keyboard::Scancode::S) {
                // Save and return to Main Menu
                // save(); // I will implement it latter
                setNextScene(SceneID::MainMenu);
                exit();
            }
        }
        if (updateRequired) {
            updateVertexArray();
        }
    }

    // void save() {
    //   to be completed
    // }

public:
    friend std::ostream &operator<<(std::ostream &os, const EditableText &obj) {
        os << "This is the EditableText class: "
                << static_cast<const Scene &>(obj)
                << ' ' << static_cast<const VertexArrayUtility &>(obj)
                << " text: " << obj.text
                << " x: " << obj.x
                << " y: " << obj.y
                << " W: " << obj.W
                << " H: " << obj.H
                << " backgroundVertexArray: " << obj.backgroundVertexArray.getVertexCount()
                << " textVertexArray: " << obj.textVertexArray.getVertexCount()
                << " cursorL: " << obj.cursorL
                << " cursorR: " << obj.cursorR
                << " selectionHead: " << obj.selectionHead
                << " step: " << obj.step
                << " ctrl: " << obj.ctrl
                << " shift: " << obj.shift;
        return os;
    }

    explicit EditableText(sf::RenderWindow *window_, std::string text_, const int16_t x_, const int16_t y_,
                          const size_t W_, const size_t H_)
        : Scene(window_), text(std::move(text_)), x(x_), y(y_), W(W_), H(H_) {
        updateVertexArray();
    }

    void manageEvent() override {
        if (!event) return;

        if (const auto *enteredText = event->getIf<sf::Event::TextEntered>()) {
            if (char c = static_cast<char>(enteredText->unicode); c == 8) {
                // Backspace
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
            // reflow the text
            updateVertexArray();
        } else if (const auto *key = event->getIf<sf::Event::KeyPressed>()) {
            if (key->scancode == sf::Keyboard::Scancode::Escape) {
                setNextScene(SceneID::MainMenu);
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
            if (k->scancode == sf::Keyboard::Scancode::LControl || k->scancode == sf::Keyboard::Scancode::RControl)
                ctrl = false;
            if (k->scancode == sf::Keyboard::Scancode::LShift || k->scancode == sf::Keyboard::Scancode::RShift)
                shift = false;
        }
    }

    void draw() override {
        window->draw(backgroundVertexArray);
        window->draw(textVertexArray);
    }

    void refresh() override {
        updateVertexArray();
    }
};

class Greet : public Scene {
    size_t W = 0;
    size_t H = 0;
    const uint8_t pixelWidth = Settings::getInstance().pixel_size();
    size_t textSize = 0;

    ReadOnlyText matrix;
    ReadOnlyText greeting;
    int16_t x = 0;

public:
    friend std::ostream &operator<<(std::ostream &os, const Greet &obj) {
        os << "This is the Greet class: "
                << static_cast<const Scene &>(obj)
                << " W: " << obj.W
                << " H: " << obj.H
                << " pixelWidth: " << obj.pixelWidth
                << " textSize: " << obj.textSize
                << " matrix: " << obj.matrix
                << " greeting: " << obj.greeting
                << " x: " << obj.x;
        return os;
    }

    explicit Greet(sf::RenderWindow *window_, const std::string &s)
        : Scene(window_) {
        textSize = s.size();
        W = window->getSize().x;
        H = window->getSize().y;

        std::string backgroundText;
        constexpr uint8_t backgroundTextLen = 8 * 8 * 3;
        uint8_t i = 0;
        while (i < backgroundTextLen) {
            for (char c = ' '; i < backgroundTextLen && c <= '~'; c++) {
                backgroundText += c;
                i++;
            }
        }

        matrix = ReadOnlyText{
            window, backgroundText, 0, 0, pixelWidth, W, H, Settings::getInstance().text_color(),
            Settings::getInstance().background_color()
        };
        greeting = ReadOnlyText{
            window,
            s,
            static_cast<int16_t>(W), 0, static_cast<uint8_t>(pixelWidth * 9),
            9 * 9 * pixelWidth * textSize, static_cast<size_t>(9 * 8 * pixelWidth),
            Settings::getInstance().text_color(),
            sf::Color::Transparent,
            false
        };

        x = static_cast<int16_t>(W);
    }

    void draw() override {
        matrix.draw();
        greeting.draw();
        x = static_cast<int16_t>(x - 9 * pixelWidth);
        greeting.moveAt(x, 0);
    }

    void manageEvent() override {
        if (x < -static_cast<int16_t>(9 * 8 * pixelWidth * textSize)) {
            setNextScene(SceneID::MainMenu);
            exit();
        }
    }

    void reset() override {
        Scene::reset();
        x = static_cast<int16_t>(W);
        greeting.moveAt(x, 0);
    }

    void update() override {
    }

    void refresh() override {
    }

    ~Greet() override = default;
};


class TilePanel : public Scene, VertexArrayUtility {
    sf::RenderWindow *window = nullptr;
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

    sf::VertexArray backgroundVertexArray;
    sf::VertexArray tableVertexArray;
    sf::VertexArray cursorVertexArray;

    void createTilePanel() {
        y = static_cast<int16_t>(Settings::getInstance().pixel_size() * 9);

        backgroundVertexArray.clear();
        backgroundVertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);
        tableVertexArray.clear();
        tableVertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);
        cursorVertexArray.clear();
        cursorVertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);

        insertRectangle(
            backgroundVertexArray,
            0, 0,
            static_cast<int16_t>(window->getSize().x),
            static_cast<int16_t>(window->getSize().y),
            Settings::getInstance().background_color()
        );

        std::ifstream input("./assets/tilePanelData.txt");
        input >> rows >> columns;

        cellW = static_cast<int16_t>(window->getSize().x / (2 * columns));
        cellH = static_cast<int16_t>(window->getSize().y / (2 * rows));

        // grid
        for (int i = 0; i <= rows; i++) {
            insertRectangle(
                tableVertexArray,
                x,
                static_cast<int16_t>(y + cellH * i),
                static_cast<int16_t>(cellW * columns + 10),
                10,
                Settings::getInstance().text_color()
            );
        }

        for (int i = 0; i <= columns; i++) {
            insertRectangle(
                tableVertexArray,
                static_cast<int16_t>(x + cellW * i),
                y,
                10,
                static_cast<int16_t>(cellH * rows),
                Settings::getInstance().text_color()
            );
        }

        int r, g, b;
        for (int j = 0; j < rows; j++) {
            for (int i = 0; i < columns; i++) {
                input >> r >> g >> b;
                colors.emplace_back(r, g, b);

                insertRectangle(
                    tableVertexArray,
                    static_cast<int16_t>(x + 10 + cellW * i),
                    static_cast<int16_t>(y + 10 + cellH * j),
                    static_cast<int16_t>(cellW - 10),
                    static_cast<int16_t>(cellH - 10),
                    sf::Color(r, g, b)
                );
            }
        }

        input.close();

        // cursor
        cursorVertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);
        // horizontal lines
        for (int i = 0; i <= 1; i++) {
            insertRectangle(cursorVertexArray, x, static_cast<int16_t>(y + cellH * i), static_cast<int16_t>(cellW + 10),
                            10, Settings::getInstance().cursor_color());
        }
        // vertical lines
        for (int i = 0; i <= 1; i++) {
            insertRectangle(cursorVertexArray, static_cast<int16_t>(x + cellW * i), y, 10, cellH,
                            Settings::getInstance().cursor_color());
        }
    }

    void manageResizedEvent() {
        titleObject.manageResizedEvent();

        backgroundVertexArray.clear();
        backgroundVertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);

        insertRectangle(
            backgroundVertexArray,
            0, 0,
            static_cast<int16_t>(window->getSize().x),
            static_cast<int16_t>(window->getSize().y),
            Settings::getInstance().background_color()
        );
    }

    void updateCursor() {
        moveAt(
            cursorVertexArray,
            static_cast<int16_t>(x + cellW * cursorX),
            static_cast<int16_t>(y + cellH * cursorY)
        );
    }

public:
    friend std::ostream &operator<<(std::ostream &os, const TilePanel &obj) {
        os << "This is the TilePanel class: "
                << static_cast<const Scene &>(obj)
                << ' ' << static_cast<const VertexArrayUtility &>(obj)
                << " window: " << obj.window
                << " titleText: " << obj.titleText
                << " target: " << obj.target
                << " titleObject: " << obj.titleObject
                << " cursorX: " << obj.cursorX
                << " cursorY: " << obj.cursorY
                << " x: " << obj.x
                << " y: " << obj.y
                << " cellW: " << obj.cellW
                << " cellH: " << obj.cellH
                << " columns: " << obj.columns
                << " rows: " << obj.rows
                << " colors: " << obj.colors.size()
                << " backgroundVertexArray: " << obj.backgroundVertexArray.getVertexCount()
                << " tableVertexArray: " << obj.tableVertexArray.getVertexCount()
                << " cursorVertexArray: " << obj.cursorVertexArray.getVertexCount();
        return os;
    }

    TilePanel(sf::RenderWindow *window_,
              std::string title_,
              const uint8_t target_)
        : Scene(window_),
          window(window_),
          titleText(std::move(title_)),
          target(target_) {
        titleObject = ReadOnlyText{
            window,
            std::move(titleText),
            0, 0,
            static_cast<uint8_t>(Settings::getInstance().pixel_size()),
            static_cast<uint16_t>(Settings::getInstance().window_w()),
            static_cast<uint16_t>(Settings::getInstance().pixel_size() * 9),
            Settings::getInstance().text_color(),
            Settings::getInstance().background_color(),
            false
        };
        createTilePanel();
    }

    void refresh() override {
        titleObject.setTextColor(Settings::getInstance().text_color());
        titleObject.setBackgroundColor(Settings::getInstance().background_color());
        titleObject.setPixelSize(static_cast<uint8_t>(Settings::getInstance().pixel_size()));
        titleObject.refresh();

        createTilePanel();
    }

    void manageEvent() override {
        if (!event) return;

        if (event->is<sf::Event::Resized>()) {
            manageResizedEvent();
            return;
        }

        if (const auto *key = event->getIf<sf::Event::KeyPressed>()) {
            switch (key->scancode) {
                case sf::Keyboard::Scancode::Right:
                    cursorX++;
                    break;

                case sf::Keyboard::Scancode::Left:
                    cursorX--;
                    break;

                case sf::Keyboard::Scancode::Up:
                    cursorY--;
                    break;

                case sf::Keyboard::Scancode::Down:
                    cursorY++;
                    break;

                case sf::Keyboard::Scancode::Enter: {
                    if (const sf::Color newColor = colors[cursorX + cursorY * columns];
                        newColor != Settings::getInstance().background_color() &&
                        newColor != Settings::getInstance().text_color() &&
                        newColor != Settings::getInstance().cursor_color()) {
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

                    setNextScene(SceneID::SettingsMenu);
                    exit();
                    return;
                }

                case sf::Keyboard::Scancode::Escape:
                    setNextScene(SceneID::SettingsMenu);
                    exit();
                    return;

                default:
                    break;
            }

            // repair overflow/underflow moves
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

    void update() override {
    }

    void draw() override {
        window->draw(backgroundVertexArray);
        titleObject.draw();
        window->draw(tableVertexArray);
        window->draw(cursorVertexArray);
    }

    void reset() override {
        cursorX = 0;
        cursorY = 0;
        requestExit = false;
        nextScene = SceneID::Exit;
    }

    ~TilePanel() override = default;
};

class Menu : public Scene, VertexArrayUtility {
    sf::RenderWindow *window = nullptr;
    ReadOnlyText text;
    std::vector<SceneID> actions;
    int cursor = 0;
    sf::VertexArray backgroundAndCursorVertexArray;

    int len = 0;
    int16_t lineHeight = 0;

    std::string parameters;

    SceneID parentID;

    void updateBackgroundAndCursor() {
        lineHeight = static_cast<int16_t>(Settings::getInstance().pixel_size() * 9);

        backgroundAndCursorVertexArray.clear();
        backgroundAndCursorVertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);

        // background
        insertRectangle(
            backgroundAndCursorVertexArray,
            0, 0,
            static_cast<int16_t>(window->getSize().x),
            static_cast<int16_t>(window->getSize().y),
            Settings::getInstance().background_color()
        );

        // cursor
        insertRectangle(
            backgroundAndCursorVertexArray,
            0,
            static_cast<int16_t>((cursor + 1) * lineHeight),
            static_cast<int16_t>(window->getSize().x),
            lineHeight,
            Settings::getInstance().cursor_color()
        );
    }

public:
    friend std::ostream &operator<<(std::ostream &os, const Menu &obj) {
        os << "This is the Menu class: "
                << static_cast<const Scene &>(obj)
                << ' ' << static_cast<const VertexArrayUtility &>(obj)
                << " window: " << obj.window
                << " text: " << obj.text
                << " actions: " << obj.actions.size()
                << " cursor: " << obj.cursor
                << " backgroundAndCursorVertexArray: " << obj.backgroundAndCursorVertexArray.getVertexCount()
                << " len: " << obj.len
                << " lineHeight: " << obj.lineHeight
                << " parameters: " << obj.parameters;
        return os;
    }

    Menu(sf::RenderWindow *window_, const std::string &menuText, const std::vector<SceneID> &actions_,
         const SceneID parentID_ = SceneID::Exit)
        : Scene(window_),
          window(window_),
          actions(actions_), parentID(parentID_) {
        len = static_cast<int>(actions.size());

        text = ReadOnlyText{
            window,
            menuText,
            0, 0,
            static_cast<uint8_t>(Settings::getInstance().pixel_size()),
            static_cast<size_t>(Settings::getInstance().window_w()),
            static_cast<size_t>(Settings::getInstance().window_h()),
            Settings::getInstance().text_color(),
            sf::Color::Transparent,
            false
        };

        updateBackgroundAndCursor();
    }

    ~Menu() override = default;

    void manageEvent() override {
        if (!event) return;

        if (event->is<sf::Event::Resized>()) {
            text.manageResizedEvent();
            updateBackgroundAndCursor();
            return;
        }

        if (const auto *key = event->getIf<sf::Event::KeyPressed>()) {
            switch (key->scancode) {
                case sf::Keyboard::Scancode::Up:
                case sf::Keyboard::Scancode::Left:
                    cursor--;
                    if (cursor < 0) cursor = len - 1;
                    updateBackgroundAndCursor();
                    break;
                case sf::Keyboard::Scancode::Down:
                case sf::Keyboard::Scancode::Right:
                    cursor++;
                    if (cursor >= len) cursor = 0;
                    updateBackgroundAndCursor();
                    break;
                case sf::Keyboard::Scancode::Enter:
                    if (cursor >= 0 && cursor < len) {
                        setNextScene(actions[cursor]);
                        exit();
                    }
                    break;
                case sf::Keyboard::Scancode::Escape:
                    setNextScene(SceneID::Exit);
                    exit();
                    break;
                case sf::Keyboard::Scancode::E:
                    setNextScene(parentID); // go to the previous menu
                    exit();
                    break;
                default:
                    break;
            }
        }
    }

    void update() override {
    }

    void draw() override {
        window->draw(backgroundAndCursorVertexArray);
        text.draw();
    }

    void refresh() override {
        text.setTextColor(Settings::getInstance().text_color());
        text.setBackgroundColor(sf::Color::Transparent);
        text.setPixelSize(Settings::getInstance().pixel_size());

        text.refresh();
        updateBackgroundAndCursor();
    }
};

class SceneManager {
    sf::RenderWindow *window;
    sf::Vector2u formerWindowSize;
    sf::Vector2i formerWindowPos;
    bool isGreetMode = false;

    std::unique_ptr<Scene> mainMenu;
    std::unique_ptr<Scene> settingsMenu;

    std::unique_ptr<Scene> backgroundSettings;
    std::unique_ptr<Scene> textSettings;
    std::unique_ptr<Scene> cursorSettings;

    std::unique_ptr<Scene> pixelSizeSettings;

    std::unique_ptr<Scene> newFile;
    std::unique_ptr<Scene> openFromDisk;
    std::unique_ptr<Scene> aiMode;
    std::unique_ptr<Scene> editor;

    Button increaseButton;
    Button decreaseButton;
    Button restoreButton;

    Scene *currentScene = nullptr;

    [[nodiscard]] Scene *resolve(const SceneID id) const {
        // static Button increaseButton(window, []() {
        //     Settings::getInstance().increase_pixel_size();
        //     Settings::getInstance().update();
        // }, SceneID::PixelSizeSettings);
        //
        // static Button decreaseButton(window, []() {
        //     Settings::getInstance().decrease_pixel_size();
        //     Settings::getInstance().update();
        // }, SceneID::PixelSizeSettings);
        //
        // static Button restoreButton(window, []() {
        //     Settings::getInstance().update(true);
        // }, SceneID::SettingsMenu);
        //
        // std::cout << "These are the buttons: "
        //  <<increaseButton << decreaseButton << restoreButton;

        switch (id) {
            case SceneID::MainMenu: return mainMenu.get();
            case SceneID::SettingsMenu: return settingsMenu.get();

            case SceneID::BackgroundSettings: return backgroundSettings.get();
            case SceneID::TextSettings: return textSettings.get();
            case SceneID::CursorSettings: return cursorSettings.get();

            case SceneID::PixelSizeSettings: return pixelSizeSettings.get();

            case SceneID::RestoreDefaults: return const_cast<Button *>(&restoreButton);
            case SceneID::Increase: return const_cast<Button *>(&increaseButton);
            case SceneID::Decrease: return const_cast<Button *>(&decreaseButton);

            case SceneID::NewFile: return newFile.get();
            case SceneID::OpenFromDisk: return openFromDisk.get();
            case SceneID::AIMode: return aiMode.get();
            case SceneID::DemoTextEditor: return editor.get();

            case SceneID::Exit:
                window->close();
                return nullptr;
            default:
                std::cout << "Unexpected state";
                window->close();
                return nullptr;
        }
    }

    void createScenes() {
        pixelSizeSettings = std::make_unique<Menu>(
            window,
            "PIXEL SIZE SETTER - E to exit\nincrease\ndecrease",
            std::vector{
                SceneID::Increase,
                SceneID::Decrease
            },
            SceneID::SettingsMenu
        );
        backgroundSettings = std::make_unique<TilePanel>(window, "Background: Enter to select", 0);
        textSettings = std::make_unique<TilePanel>(window, "Text: Enter to select", 1);
        cursorSettings = std::make_unique<TilePanel>(window, "Cursor: Enter to select", 2);

        settingsMenu = std::make_unique<Menu>(
            window,
            "SETTINGS MENU - E to exit\nbackground color\ntext color\ncursor color\npixel size\nrestore defaults",
            std::vector{
                SceneID::BackgroundSettings,
                SceneID::TextSettings,
                SceneID::CursorSettings,
                SceneID::PixelSizeSettings,
                SceneID::RestoreDefaults,
            },
            SceneID::MainMenu
        );

        newFile = std::make_unique<Greet>(window, "new file");
        openFromDisk = std::make_unique<Greet>(window, "open file");
        aiMode = std::make_unique<Greet>(window, "malware successfully installed");

        // will be modified with paths and other things like that
        std::ifstream input("tastatura.txt");
        const std::string s(
            (std::istreambuf_iterator<char>(input)),
            std::istreambuf_iterator<char>()
        );
        input.close();
        editor = std::make_unique<EditableText>(window, s, 0, 0,
                                                window->getSize().x,
                                                window->getSize().y);

        mainMenu = std::make_unique<Menu>(
            window,
            "MAIN MENU - E to exit\nsettings\nnew file *ISN'T WORKING YET*\nopen from disk *ISN'T WORKING YET*\nAI mode *ISN'T WORKING YET*\ndemo text editor",
            std::vector{
                SceneID::SettingsMenu,
                SceneID::NewFile,
                SceneID::OpenFromDisk,
                SceneID::AIMode,
                SceneID::DemoTextEditor
            },
            SceneID::Exit
        );
    }

public:
    friend std::ostream &operator<<(std::ostream &os, const SceneManager &obj) {
        os << "\n---------------------------------------------------------------------------------------"
                << "\nThis is the SceneManager class: "
                << "\n---------------------------------------------------------------------------------------"
                << "\nwindow: " << obj.window
                << "\n---------------------------------------------------------------------------------------"
                << "\noriginalWindowSize: " << obj.formerWindowSize.x << ", " << obj.formerWindowSize.y
                << "\n---------------------------------------------------------------------------------------"
                << "\nisGreetMode: " << obj.isGreetMode
                << "\n---------------------------------------------------------------------------------------"
                << "\nmainMenu: " << dynamic_cast<const Menu &>(*obj.mainMenu)
                << "\n---------------------------------------------------------------------------------------"
                << "\nsettingsMenu: " << dynamic_cast<const Menu &>(*obj.settingsMenu)
                << "\n---------------------------------------------------------------------------------------"
                << "\nbackgroundSettings: " << dynamic_cast<const TilePanel &>(*obj.backgroundSettings)
                << "\n---------------------------------------------------------------------------------------"
                << "\ntextSettings: " << dynamic_cast<const TilePanel &>(*obj.textSettings)
                << "\n---------------------------------------------------------------------------------------"
                << "\ncursorSettings: " << dynamic_cast<const TilePanel &>(*obj.cursorSettings)
                << "\n---------------------------------------------------------------------------------------"
                << "\npixelSizeSettings: " << dynamic_cast<const Menu &>(*obj.pixelSizeSettings)
                << "\n---------------------------------------------------------------------------------------"
                << "\nnewFile: " << dynamic_cast<const Greet &>(*obj.newFile)
                << "\n---------------------------------------------------------------------------------------"
                << "\nopenFromDisk: " << dynamic_cast<const Greet &>(*obj.openFromDisk)
                << "\n---------------------------------------------------------------------------------------"
                << "\naiMode: " << dynamic_cast<const Greet &>(*obj.aiMode)
                << "\n---------------------------------------------------------------------------------------"
                << "\neditor: " << dynamic_cast<const EditableText &>(*obj.editor)
                << "\n---------------------------------------------------------------------------------------"
                << "\ncurrentScene: " << *obj.currentScene
                << "\n---------------------------------------------------------------------------------------"
                << "\nincreaseButton: " << obj.increaseButton
                << "\n---------------------------------------------------------------------------------------"
                << "\ndecreaseButton: " << obj.decreaseButton
                << "\n---------------------------------------------------------------------------------------"
                << "\nrestoreButton: " << obj.restoreButton
                << "\n---------------------------------------------------------------------------------------";
        return os;
    }

    explicit SceneManager(sf::RenderWindow *window_)
        : window(window_),
          increaseButton(window_, []() {
              Settings::getInstance().increase_pixel_size();
              Settings::getInstance().update();
          }, SceneID::PixelSizeSettings),
          decreaseButton(window_, []() {
              Settings::getInstance().decrease_pixel_size();
              Settings::getInstance().update();
          }, SceneID::PixelSizeSettings),
          restoreButton(window_, []() { Settings::getInstance().update(true); }, SceneID::SettingsMenu) {
        createScenes();
        currentScene = mainMenu.get();
    }

    void run() {
        while (window->isOpen()) {
            while (auto e = window->pollEvent()) {
                if (e->is<sf::Event::Closed>()) {
                    window->close();
                    return;
                }
                if (const auto *resized = e->getIf<sf::Event::Resized>()) {
                    std::cout << "New width: " << window->getSize().x << '\n'
                            << "New height: " << window->getSize().y << '\n';
                    // update the view to the new size of the window
                    sf::FloatRect visibleArea({0.f, 0.f}, sf::Vector2f(resized->size));
                    window->setView(sf::View(visibleArea));
                }
                currentScene->manageEvent(*e);
            }

            currentScene->update();

            window->clear();
            currentScene->draw();
            window->display();

            if (currentScene->hasFinished()) {
                const SceneID next = currentScene->getNextScene();
                currentScene->reset();

                const bool nextIsGreet = (next == SceneID::NewFile ||
                                          next == SceneID::OpenFromDisk ||
                                          next == SceneID::AIMode);

                if (nextIsGreet && !isGreetMode) {
                    window->setFramerateLimit(20);
                    formerWindowSize = window->getSize();

                    const int W = Settings::getInstance().pixel_size() * 9 * 8 * 3;
                    const int H = Settings::getInstance().pixel_size() * 9 * 8;

                    formerWindowPos = window->getPosition();

                    sf::Vector2i desiredPos;
                    desiredPos.x = static_cast<int>(formerWindowPos.x + (window->getSize().x - W) / 2);
                    desiredPos.y = static_cast<int>(formerWindowPos.y + (window->getSize().y - H) / 2);

                    // window->create(sf::VideoMode({static_cast<unsigned int>(W), static_cast<unsigned int>(H)}),
                    //                "", sf::Style::None, sf::State::Windowed);
                    // window->setPosition(desiredPos);

                    window->setSize({static_cast<unsigned int>(W), static_cast<unsigned int>(H)});
                    // const int newX = window->getPosition().x;
                    // const int newY = window->getPosition().y;
                    window->setPosition(desiredPos);

                    isGreetMode = true;
                } else if (!nextIsGreet && isGreetMode) {
                    window->setFramerateLimit(10);
                    window->setSize(formerWindowSize);
                    window->setPosition(formerWindowPos);
                    isGreetMode = false;
                }

                Scene *nextPtr = resolve(next);
                if (!nextPtr) return;

                currentScene = nextPtr;
                currentScene->refresh();
            }
        }
    }
};


int main() {
    sf::RenderWindow window;
    window.create(sf::VideoMode({800, 700}), "car_plus_plus", sf::Style::Default, sf::State::Windowed);
    std::cout << "The window was created successfully\n";
    window.setFramerateLimit(10);

    SceneManager sm(&window);

    // compunerea cu operator<< se vede in clasa SceneManager, care afiseaza mai multe obiecte
    std::cout << "Afisare: " << sm << "\nwindow width: " << window.getSize().x << ", window height: " << window.
            getSize().y << '\n';

    sm.run();

    std::cout << "The program finished successfully\n";
    return 0;
}
