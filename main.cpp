#include <iostream>
#include <fstream>
#include <ostream>
#include <thread>

#include <SFML/Graphics.hpp>
#include <utility>

const uint8_t ascii[8 * 95] = {
    0b00011000,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00000000,
    0b00011000,
    0b00011000,

    0b01101100,
    0b01101100,
    0b01101100,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    0b00100100,
    0b00100100,
    0b11111111,
    0b00100100,
    0b00100100,
    0b11111111,
    0b00100100,
    0b00100100,

    0b00011000,
    0b11111111,
    0b10011000,
    0b11111111,
    0b00011001,
    0b00011001,
    0b11111111,
    0b00011000,

    0b01100001,
    0b10010010,
    0b10010100,
    0b01101000,
    0b00010110,
    0b00101001,
    0b01001001,
    0b10000110,

    0b01110000,
    0b10001000,
    0b10001000,
    0b01110001,
    0b10001011,
    0b10000110,
    0b10000110,
    0b01111011,

    0b00011000,
    0b00011000,
    0b00011000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    0b00001000,
    0b00011000,
    0b00110000,
    0b00110000,
    0b00110000,
    0b00110000,
    0b00011000,
    0b00001000,

    0b00100000,
    0b00110000,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00110000,
    0b00100000,

    0b00011000,
    0b10011001,
    0b11011011,
    0b00111100,
    0b00111100,
    0b11011011,
    0b10011001,
    0b00011000,

    0b00000000,
    0b00011000,
    0b00011000,
    0b01111110,
    0b01111110,
    0b00011000,
    0b00011000,
    0b00000000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00001100,
    0b00011000,
    0b00110000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b01111110,
    0b01111110,
    0b00000000,
    0b00000000,
    0b00000000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00111000,
    0b00111000,
    0b00111000,

    0b00000001,
    0b00000011,
    0b00000110,
    0b00001100,
    0b00011000,
    0b00110000,
    0b01100000,
    0b11000000,

    0b00111100,
    0b01000010,
    0b01000110,
    0b01001010,
    0b01010010,
    0b01100010,
    0b01000010,
    0b00111100,

    0b00011000,
    0b00111000,
    0b01111000,
    0b00011000,
    0b00011000,
    0b00011000,
    0b01111110,
    0b01111110,

    0b00111100,
    0b01111110,
    0b01100110,
    0b00001100,
    0b00011000,
    0b00110000,
    0b01111110,
    0b01111110,

    0b00111100,
    0b01000110,
    0b00000110,
    0b00111100,
    0b00000110,
    0b01000110,
    0b01111110,
    0b00111100,

    0b00000100,
    0b00001100,
    0b00011100,
    0b00101100,
    0b01001100,
    0b01111110,
    0b00001100,
    0b00001100,

    0b01111110,
    0b01100000,
    0b01100000,
    0b01111100,
    0b00000110,
    0b00000110,
    0b01111110,
    0b00111100,

    0b00111100,
    0b01100110,
    0b01000000,
    0b01111100,
    0b01000010,
    0b01000010,
    0b01111110,
    0b00111100,

    0b01111110,
    0b01000110,
    0b00000110,
    0b00001100,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00011000,

    0b00111100,
    0b01000010,
    0b01000010,
    0b00111100,
    0b01000010,
    0b01000010,
    0b01000010,
    0b00111100,

    0b00111100,
    0b01000010,
    0b01000010,
    0b00111110,
    0b00000010,
    0b00000010,
    0b01000010,
    0b00111100,

    0b00000000,
    0b00000000,
    0b00111000,
    0b00111000,
    0b00000000,
    0b00111000,
    0b00111000,
    0b00000000,

    0b00000000,
    0b00000000,
    0b00111000,
    0b00111000,
    0b00000000,
    0b00011000,
    0b00110000,
    0b00100000,

    0b00000000,
    0b00000000,
    0b00001110,
    0b00111000,
    0b11100000,
    0b00111000,
    0b00001110,
    0b00000000,

    0b00000000,
    0b00000000,
    0b11111111,
    0b11111111,
    0b00000000,
    0b11111111,
    0b11111111,
    0b00000000,

    0b00000000,
    0b00000000,
    0b01110000,
    0b00011100,
    0b00000111,
    0b00011100,
    0b01110000,
    0b00000000,

    0b01111110,
    0b11000011,
    0b10000011,
    0b00011110,
    0b00011000,
    0b00000000,
    0b00011000,
    0b00011000,

    0b00111110,
    0b01000001,
    0b10001111,
    0b10010001,
    0b10010001,
    0b10001111,
    0b01000000,
    0b00111111,

    0b11111111,
    0b11111111,
    0b11000011,
    0b11000011,
    0b11111111,
    0b11000011,
    0b11000011,
    0b11000011,

    0b11111110,
    0b11000011,
    0b11000011,
    0b11111110,
    0b11000011,
    0b11000011,
    0b11000011,
    0b11111110,

    0b01111110,
    0b11100111,
    0b11000011,
    0b11000000,
    0b11000000,
    0b11000011,
    0b11100111,
    0b01111110,

    0b11111100,
    0b11000110,
    0b11000011,
    0b11000011,
    0b11000011,
    0b11000011,
    0b11000110,
    0b11111100,

    0b11111111,
    0b11111111,
    0b11000000,
    0b11111110,
    0b11000000,
    0b11000000,
    0b11111111,
    0b11111111,

    0b11111110,
    0b11111110,
    0b11000000,
    0b11111100,
    0b11000000,
    0b11000000,
    0b11000000,
    0b11000000,

    0b01111100,
    0b11000010,
    0b11000000,
    0b11000000,
    0b11001111,
    0b11000011,
    0b11111111,
    0b01111110,

    0b11000011,
    0b11000011,
    0b11000011,
    0b11111111,
    0b11111111,
    0b11000011,
    0b11000011,
    0b11000011,

    0b11111111,
    0b11111111,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00011000,
    0b11111111,
    0b11111111,

    0b01111110,
    0b01111110,
    0b00001100,
    0b00001100,
    0b00001100,
    0b11001100,
    0b11111100,
    0b01111000,

    0b11000110,
    0b11001100,
    0b11011000,
    0b11110000,
    0b11110000,
    0b11011000,
    0b11001100,
    0b11000110,

    0b11000000,
    0b11000000,
    0b11000000,
    0b11000000,
    0b11000000,
    0b11000000,
    0b11111111,
    0b11111111,

    0b11000001,
    0b11100011,
    0b11010101,
    0b11001001,
    0b11000001,
    0b11000001,
    0b11000001,
    0b11000001,

    0b11000001,
    0b11000001,
    0b11100001,
    0b11110001,
    0b11011001,
    0b11001101,
    0b11000111,
    0b11000001,

    0b01111110,
    0b11000011,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b11000011,
    0b01111110,

    0b11111110,
    0b11000011,
    0b11000001,
    0b11000011,
    0b11111110,
    0b11000000,
    0b11000000,
    0b11000000,

    0b01111110,
    0b11000011,
    0b10000001,
    0b10000001,
    0b10011001,
    0b10001101,
    0b11000110,
    0b01111011,

    0b11111110,
    0b11000011,
    0b11000001,
    0b11000011,
    0b11111110,
    0b11011100,
    0b11001110,
    0b11000111,

    0b01111110,
    0b11000011,
    0b11000011,
    0b01110000,
    0b00011110,
    0b11000011,
    0b11000011,
    0b01111110,

    0b11111111,
    0b11111111,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00011000,

    0b11000011,
    0b11000011,
    0b11000011,
    0b11000011,
    0b11000011,
    0b11000011,
    0b11111111,
    0b01111110,

    0b11000001,
    0b11000001,
    0b11000001,
    0b11000001,
    0b11000010,
    0b01100100,
    0b00111000,
    0b00010000,

    0b11000001,
    0b11000001,
    0b11000001,
    0b11000001,
    0b11010101,
    0b11001001,
    0b01010101,
    0b00100010,

    0b11000011,
    0b01100110,
    0b00111100,
    0b00011000,
    0b00011000,
    0b00111100,
    0b01100110,
    0b11000011,

    0b11000011,
    0b01100110,
    0b00111100,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00011000,

    0b11111111,
    0b11111111,
    0b00001100,
    0b00011000,
    0b00110000,
    0b01100000,
    0b11111111,
    0b11111111,

    0b00111100,
    0b00111100,
    0b00110000,
    0b00110000,
    0b00110000,
    0b00110000,
    0b00111100,
    0b00111100,

    0b11000000,
    0b01100000,
    0b00110000,
    0b00011000,
    0b00001100,
    0b00000110,
    0b00000011,
    0b00000001,

    0b00111100,
    0b00111100,
    0b00001100,
    0b00001100,
    0b00001100,
    0b00001100,
    0b00111100,
    0b00111100,

    0b00111000,
    0b01101100,
    0b11000110,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b11111111,
    0b11111111,

    0b00110000,
    0b00011000,
    0b00001100,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,

    0b00000000,
    0b00000000,
    0b01111110,
    0b00000011,
    0b01111111,
    0b10000011,
    0b10000011,
    0b01111111,

    0b11000000,
    0b11000000,
    0b11000000,
    0b11111110,
    0b11000011,
    0b11000001,
    0b11000011,
    0b11111110,

    0b00000000,
    0b00000000,
    0b01111110,
    0b10000001,
    0b10000000,
    0b10000000,
    0b10000001,
    0b01111110,

    0b00000011,
    0b00000011,
    0b00000011,
    0b01111111,
    0b11000011,
    0b10000011,
    0b11000011,
    0b01111111,

    0b00000000,
    0b00000000,
    0b01111110,
    0b10000001,
    0b11111111,
    0b10000000,
    0b10000001,
    0b01111110,

    0b00001110,
    0b00011111,
    0b00011000,
    0b01111111,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00011000,

    0b00000000,
    0b00000000,
    0b00111101,
    0b01000011,
    0b01000011,
    0b00111101,
    0b10000001,
    0b01111110,

    0b11000000,
    0b11000000,
    0b11000000,
    0b11111100,
    0b11111110,
    0b11000011,
    0b11000011,
    0b11000011,

    0b00011000,
    0b00011000,
    0b00000000,
    0b00111100,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00111100,

    0b00001100,
    0b00001100,
    0b00000000,
    0b00011110,
    0b00001100,
    0b01101100,
    0b01101100,
    0b00111000,

    0b11000000,
    0b11000010,
    0b11000100,
    0b11001000,
    0b11011000,
    0b11100100,
    0b11000010,
    0b11000001,

    0b11110000,
    0b11110000,
    0b00110000,
    0b00110000,
    0b00110000,
    0b00110000,
    0b00111111,
    0b00011111,

    0b00000000,
    0b00000000,
    0b11111111,
    0b11001001,
    0b11001001,
    0b11001001,
    0b11001001,
    0b11001001,

    0b00000000,
    0b00000000,
    0b11011100,
    0b11100011,
    0b11000001,
    0b11000001,
    0b11000001,
    0b11000001,

    0b00000000,
    0b00000000,
    0b01111100,
    0b11100011,
    0b11000001,
    0b11000001,
    0b11100011,
    0b01111100,

    0b00000000,
    0b00000000,
    0b11111110,
    0b11000001,
    0b11000001,
    0b11111110,
    0b11000000,
    0b11000000,

    0b00000000,
    0b00000000,
    0b01111111,
    0b10000011,
    0b10000011,
    0b01111111,
    0b00000011,
    0b00000011,

    0b00000000,
    0b00000000,
    0b11011100,
    0b11100010,
    0b11000000,
    0b11000000,
    0b11000000,
    0b11000000,

    0b00000000,
    0b00000000,
    0b01111110,
    0b10000000,
    0b01111110,
    0b00000001,
    0b10000001,
    0b01111110,

    0b00011000,
    0b00011000,
    0b01111110,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00011110,
    0b00001110,

    0b00000000,
    0b00000000,
    0b10000011,
    0b10000011,
    0b10000011,
    0b10000011,
    0b11000111,
    0b01111011,

    0b00000000,
    0b00000000,
    0b11000001,
    0b11000001,
    0b11000010,
    0b01100100,
    0b00111000,
    0b00010000,

    0b00000000,
    0b00000000,
    0b11000001,
    0b11000001,
    0b11010101,
    0b11001001,
    0b01010101,
    0b00100010,

    0b00000000,
    0b00000000,
    0b11000011,
    0b01100110,
    0b00011000,
    0b01100110,
    0b11000011,
    0b11000011,

    0b00000000,
    0b00000000,
    0b11000011,
    0b01100110,
    0b00111100,
    0b00011000,
    0b00110000,
    0b11100000,

    0b00000000,
    0b00000000,
    0b11111110,
    0b00001100,
    0b00011000,
    0b00110000,
    0b01100000,
    0b11111110,

    0b00000000,
    0b00001110,
    0b00010000,
    0b00010000,
    0b01100000,
    0b00010000,
    0b00010000,
    0b00001110,

    0b00011000,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00011000,

    0b00000000,
    0b01110000,
    0b00001000,
    0b00001000,
    0b00000110,
    0b00001000,
    0b00001000,
    0b01110000,

    0b00000000,
    0b00000000,
    0b01110001,
    0b10001001,
    0b10000110,
    0b00000000,
    0b00000000,
    0b00000000,

    0b10101010,
    0b01010101,
    0b10101010,
    0b01010101,
    0b10101010,
    0b01010101,
    0b10101010,
    0b01010101
};

class VertexArrayUtility {
public:
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

    static void insertChar(sf::VertexArray &v, const char c, const int16_t x, const int16_t y, const int16_t pixelSize,
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

class ReadOnlyText {
    friend std::ostream &operator<<(std::ostream &os, const ReadOnlyText &obj) {
        return os
               << "\nIf you really want to see the contents of this class, take a look at the following lines:"
               << "\ntext: " << obj.text
               << "\nx: " << obj.x
               << "\ny: " << obj.y
               << "\npixelSize: " << obj.pixelSize
               << "\nW: " << obj.W
               << "\nH: " << obj.H
               << "\ncolor: rgba(" << obj.textColor.r << ", " << obj.textColor.g << ", " << obj.textColor.b << ", " <<
               obj.textColor.a
               << ")"
               << "\nbackground: rgba(" << obj.backgroundColor.r << ", " << obj.backgroundColor.g << ", " << obj.
               backgroundColor.b <<
               ", " << obj.backgroundColor.a << ")"
               << "\nlineWrappable: " << obj.lineWrappable
               << "\nbg (the vertex count): " << obj.backgroundVertexArray.getVertexCount()
               << "\ntextVertexArray (the vertex count): " << obj.textVertexArray.getVertexCount()
               << "\n";
    }

    sf::RenderWindow *window;
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
        VertexArrayUtility::insertRectangle(backgroundVertexArray, x, y, static_cast<int16_t>(W),
                                            static_cast<int16_t>(H), backgroundColor);

        if (lineWrappable) {
            std::string formattedText = text;
            const auto charPoz = static_cast<int16_t>(9 * pixelSize);
            const auto rightLimit = static_cast<int16_t>(W + x - charPoz);
            const size_t rowLen = (W + x) / charPoz;

            int16_t i = 0;
            while (i < static_cast<int16_t>(formattedText.size())) {
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
            const auto size = static_cast<int16_t>(formattedText.size());
            for (int16_t r = y; i < size; r = static_cast<int16_t>(r + charPoz)) {
                for (int16_t c = x; c <= rightLimit && i < size; c = static_cast<int16_t>(c + charPoz)) {
                    VertexArrayUtility::insertChar(textVertexArray, formattedText[i], c, r, pixelSize, textColor);
                    i++;
                }
            }
        } else {
            std::string formattedText = text;
            const auto charPoz = static_cast<int16_t>(9 * pixelSize);

            int16_t i = 0;
            while (i < static_cast<int16_t>(formattedText.size())) {
                if (formattedText[i] == '\t') {
                    formattedText[i] = ' ';
                    formattedText.insert(i, "   ");
                    i += 4;
                } else {
                    i++;
                }
            }

            i = 0;
            const auto size = static_cast<int16_t>(formattedText.size());
            int16_t r = y;
            while (i < size) {
                int16_t c = x;
                while (i < size && formattedText[i] != '\n') {
                    VertexArrayUtility::insertChar(textVertexArray, formattedText[i], c, r, pixelSize, textColor);
                    i++;
                    c = static_cast<int16_t>(c + charPoz);
                }
                if (formattedText[i] == '\n') {
                    i++;
                }
                r = static_cast<int16_t>(r + charPoz);
            }
        }
    }

public:
    ReadOnlyText() : lineWrappable(true) {
        window = nullptr;
        x = 0;
        y = 0;
        pixelSize = 5;
        W = 0;
        H = 0;
    };

    explicit ReadOnlyText(sf::RenderWindow *window_, std::string text_, const int16_t x_, const int16_t y_,
                          const uint8_t pixelSize_,
                          const size_t W_, const size_t H_, const sf::Color color_,
                          const sf::Color background_, const bool lineWrappable_ = true) : text(std::move(text_)),
        x(x_), y(y_),
        pixelSize(pixelSize_), W(W_), H(H_),
        textColor(color_),
        backgroundColor(background_), lineWrappable(lineWrappable_) {
        window = window_;
        updateVertexArray();
    }

    ReadOnlyText(const ReadOnlyText &other) = default;

    ReadOnlyText(ReadOnlyText &&other) noexcept
        : window(other.window),
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
        window = other.window;
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
        window = other.window;
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

    void draw() const {
        window->draw(backgroundVertexArray);
        window->draw(textVertexArray);
    }

    void manageResizedEvent() {
        //reflow the text
        if (lineWrappable) {
            W = window->getSize().x;
            H = window->getSize().y;
            updateVertexArray();
        }
    }

    void moveAt(const int16_t newX, const int16_t newY) {
        const auto stepX = static_cast<int16_t>(newX - x);
        const auto stepY = static_cast<int16_t>(newY - y);
        const size_t count = textVertexArray.getVertexCount();
        for (size_t i = 0; i < count; i++) {
            textVertexArray[i].position.x += static_cast<float>(stepX);
            textVertexArray[i].position.x += static_cast<float>(stepY);
        }
        x = newX;
        y = newY;
    }
};

class EditableText {
    friend std::ostream &operator<<(std::ostream &os, const EditableText &obj) {
        return os
               << "\nIf you really want to see the contents of this class, take a look at the following lines:"
               << "\nwindow: " << obj.window
               << "\ntext: " << obj.text
               << "\nx: " << obj.x
               << "\ny: " << obj.y
               << "\npixelSize: " << obj.pixelSize
               << "\nW: " << obj.W
               << "\nH: " << obj.H
               << "\ncolor: rgba(" << obj.textColor.r << ", " << obj.textColor.g << ", " << obj.textColor.b << ", " <<
               obj.textColor.a
               << ")"
               << "\nbackground: rgba(" << obj.backgroundColor.r << ", " << obj.backgroundColor.g << ", " << obj.
               backgroundColor.b <<
               ", " << obj.backgroundColor.a << ")"
               << "\nbg (the vertex count): " << obj.backgroundVertexArray.getVertexCount()
               << "\ntextVertexArray (the vertex count): " << obj.textVertexArray.getVertexCount()
               << "\ncursorL: " << obj.cursorL
               << "\ncursorR: " << obj.cursorR
               << "\nselectionHead: " << obj.selectionHead
               << "\nstep: " << obj.step << "\n";
    }

    sf::RenderWindow *window;
    std::string text;
    int16_t x;
    int16_t y;
    int16_t pixelSize;
    size_t W;
    size_t H;
    sf::Color textColor;
    sf::Color backgroundColor;
    sf::Color cursorColor;

    sf::VertexArray backgroundVertexArray;
    sf::VertexArray textVertexArray;

    size_t cursorL = 0;
    size_t cursorR = 0;
    size_t *selectionHead = &cursorR;
    size_t step = 0;

    void updateVertexArray() {
        textVertexArray.clear();
        textVertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);

        backgroundVertexArray.clear();
        backgroundVertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);
        VertexArrayUtility::insertRectangle(backgroundVertexArray, x, y, static_cast<int16_t>(W),
                                            static_cast<int16_t>(H), backgroundColor);

        std::string formattedText = text;
        const size_t charPoz = 9 * pixelSize;
        const size_t rightLimit = W + x - charPoz;
        step = (W + x) / charPoz;

        size_t formattedCursorL = cursorL;
        size_t formattedCursorR = cursorR;

        size_t i = 0;
        while (i < formattedText.size()) {
            if (formattedText[i] == '\t') {
                if (i < formattedCursorL) {
                    formattedCursorL += 3;
                }
                if (i < formattedCursorR) {
                    formattedCursorR += 3;
                }
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
            formattedText += " "; // the space helps to display the cursor if it is at the end of the text
        }

        size_t visibleTextL = 0;
        size_t visibleTextR = formattedText.size();

        // if the text doesn't fit the window
        const size_t maxChars = (W * H) / (charPoz * charPoz);
        size_t focusCursor;
        if (selectionHead == &cursorL) {
            focusCursor = formattedCursorL;
        } else {
            focusCursor = formattedCursorR;
        }
        while (focusCursor - visibleTextL + step >= maxChars) {
            visibleTextL += step;
        }
        if (visibleTextL + maxChars < visibleTextR) {
            visibleTextR = visibleTextL + maxChars;
        }

        for (size_t r = y; visibleTextL < visibleTextR; r += charPoz) {
            for (size_t c = x; c <= rightLimit && visibleTextL < visibleTextR; c += charPoz) {
                if (formattedCursorL <= visibleTextL && visibleTextL <= formattedCursorR) {
                    // This happens if the char has cursor focus
                    VertexArrayUtility::insertRectangle(textVertexArray, static_cast<int16_t>(c),
                                                        static_cast<int16_t>(r), static_cast<int16_t>(pixelSize * 9),
                                                        static_cast<int16_t>(pixelSize * 9), cursorColor);
                }
                if ('!' <= formattedText[visibleTextL] && formattedText[visibleTextL] <= '~') {
                    VertexArrayUtility::insertChar(textVertexArray, formattedText[visibleTextL],
                                                   static_cast<int16_t>(c), static_cast<int16_t>(r), pixelSize,
                                                   textColor);
                }

                visibleTextL++;
            }
        }
    }

public:
    explicit EditableText(sf::RenderWindow *window_, std::string text_, const int16_t x_, const int16_t y_,
                          const uint8_t pixelSize_,
                          const size_t W_, const size_t H_, const sf::Color color_,
                          const sf::Color background_, const sf::Color cursorColor_) : window(window_),
        text(std::move(text_)), x(x_),
        y(y_),
        pixelSize(pixelSize_), W(W_), H(H_),
        textColor(color_),
        backgroundColor(background_), cursorColor(cursorColor_) {
        updateVertexArray();
    }

    ~EditableText() = default;

    void draw() const {
        window->draw(backgroundVertexArray);
        window->draw(textVertexArray);
    }

    void manageResizedEvent() {
        //reflow the text
        W = window->getSize().x;
        H = window->getSize().y;
        updateVertexArray();
    }

    void moveAt(const int16_t newX, const int16_t newY) {
        x = newX;
        y = newY;
        updateVertexArray();
    }

    void deleteSelectedText() {
        if (cursorL != cursorR) {
            text.erase(cursorL, cursorR - cursorL + 1);
            cursorR = cursorL;
        }
    }

    void manageEnteredText(const sf::Event::TextEntered *enteredText) {
        std::string c;
        if (enteredText->unicode < 128) {
            if (const char k = static_cast<char>(enteredText->unicode); k == 8) {
                if (!text.empty()) {
                    if (cursorL && cursorL == cursorR) {
                        text.erase(--cursorL, 1);
                        cursorR = cursorL;
                    } else {
                        deleteSelectedText();
                    }
                }
            } else {
                c = " ";
                c[0] = k;
            }
        } else {
            c = " ";
            c[0] = 8;
            // 8 == backspace. This manages to insert the "unknown char" symbol for any other char than those dealt with above
        }
        if (!c.empty()) {
            deleteSelectedText();

            text.insert(cursorR++, c);
            cursorL++;
        }
        updateVertexArray();
    }

    void manageKey(const sf::Keyboard::Scancode k, const bool ctrl, const bool shift) {
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
            }
        }
        if (updateRequired) {
            updateVertexArray();
        }
    }
};

class Greet {
    friend std::ostream &operator<<(std::ostream &os, const Greet &obj) {
        return os
               << "\nIf you really want to see the contents of this class, take a look at the following lines:"
               << "\nformerPos(x, y): " << obj.formerPos.x << ", " << obj.formerPos.y
               << "\nformerWindowW: " << obj.formerWindowW
               << "\nformerWindowH: " << obj.formerWindowH
               << "\nwindow: " << obj.window
               << "\nW: " << obj.W
               << "\nH: " << obj.H
               << "\npixelWidth: " << obj.pixelWidth
               << "\ntextSize: " << obj.textSize
               << "\nmatrix: " << obj.matrix
               << "\ngreeting: " << obj.greeting
               << "\nx: " << obj.x;
    }

    sf::Vector2i formerPos;
    unsigned int formerWindowW = 0;
    unsigned int formerWindowH = 0;
    sf::RenderWindow *window;
    size_t W;
    size_t H;
    const uint8_t pixelWidth = 2;
    size_t textSize;

    ReadOnlyText matrix;
    ReadOnlyText greeting;

    int16_t x;

    void setProperties() {
        formerWindowW = window->getSize().x;
        formerWindowH = window->getSize().y;
        formerPos = window->getPosition();

        sf::Vector2i desiredPos;
        desiredPos.x = static_cast<int>(formerPos.x + (window->getSize().x - W) / 2);
        desiredPos.y = static_cast<int>(formerPos.y + (window->getSize().y - H) / 2);

        window->create(sf::VideoMode({static_cast<unsigned int>(W), static_cast<unsigned int>(H)}),
                       "", sf::Style::None, sf::State::Windowed);
        window->setPosition(desiredPos);
        window->setFramerateLimit(20);
    }

    void resetProperties() {
        window->create(sf::VideoMode({formerWindowW, formerWindowH}), "car_plus_plus", sf::Style::Default,
                       sf::State::Windowed);
        window->setFramerateLimit(10);
    }

public:
    explicit Greet(sf::RenderWindow *window_, const std::string &s) {
        textSize = s.size();

        W = pixelWidth * 9 * 8 * 3;
        H = pixelWidth * 9 * 8;

        window = window_;

        setProperties();

        x = static_cast<int16_t>(W);

        std::string backgroundText;
        constexpr uint8_t backgroundTextLen = 8 * 8 * 3;
        uint8_t i = 0;
        while (i < backgroundTextLen) {
            for (char c = ' '; i < backgroundTextLen && c <= '~'; c++) {
                backgroundText += c;
                i++;
            }
        }

        matrix = ReadOnlyText{window, backgroundText, 0, 0, pixelWidth, W, H, sf::Color::Black, sf::Color::White};
        greeting = ReadOnlyText{
            window,
            s, static_cast<int16_t>(W), 0, static_cast<uint8_t>(pixelWidth * 9),
            9 * 9 * pixelWidth * textSize, static_cast<size_t>(9 * 8 * pixelWidth), sf::Color::Black,
            sf::Color::Transparent
        };
    }

    ~Greet() = default;

    int draw() {
        matrix.draw();
        greeting.draw();

        x = static_cast<int16_t>(x - 9 * pixelWidth);
        if (x < -static_cast<int16_t>(9 * 8 * pixelWidth * textSize)) {
            resetProperties();
            return 0;
        }
        greeting.moveAt(x, 0);
        return 1;
    }

    void play() {
        while (window->isOpen()) {
            while (const std::optional event = window->pollEvent()) {
                if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                        window->close();
                        std::cout << "Window closed\n";
                    }
                }
            }

            window->clear();

            if (!draw()) {
                break;
            }

            window->display();
        }
    }
};

class Menu {
    friend std::ostream &operator<<(std::ostream &os, const Menu &obj) {
        return os
               << "If you really want to see the contents of this class, take a look at the following lines:"
               << "window: " << obj.window
               << " text: " << obj.text
               << "\ncolor: rgba(" << obj.backgroundColor.r << ", " << obj.backgroundColor.g << ", " << obj.
               backgroundColor.b << ", " << obj.backgroundColor.a << ")"
               << "\ncursorColor: rgba(" << obj.cursorColor.r << ", " << obj.cursorColor.g << ", " << obj.
               cursorColor.b << ", " << obj.cursorColor.a << ")"
               << " actions size: " << obj.actions.size()
               << " cursor: " << obj.cursor
               << " backgroundAndCursorVertexArray (vertexCount): " << obj.backgroundAndCursorVertexArray.getVertexCount()
               << " len: " << obj.len
               << " lineHeight: " << obj.lineHeight;
    }

    sf::RenderWindow *window;
    ReadOnlyText text;
    const sf::Color backgroundColor;
    const sf::Color cursorColor;
    std::vector<int8_t> actions;
    int16_t cursor = 0;
    sf::VertexArray backgroundAndCursorVertexArray;
    int16_t len;
    int16_t lineHeight;

    void updateBackgroundAndCursor() {
        backgroundAndCursorVertexArray.clear();
        //background
        VertexArrayUtility::insertRectangle(backgroundAndCursorVertexArray, 0, 0,
                                            static_cast<int16_t>(window->getSize().x),
                                            static_cast<int16_t>(window->getSize().y), backgroundColor);
        //cursor
        VertexArrayUtility::insertRectangle(backgroundAndCursorVertexArray, 0,
                                            static_cast<int16_t>((cursor + 1) * lineHeight),
                                            static_cast<int16_t>(window->getSize().x), lineHeight, cursorColor);
    }

    void manageResizedEvent() {
        text.manageResizedEvent();
        updateBackgroundAndCursor();
    }

public:
    explicit Menu(sf::RenderWindow *window_, std::string parameters_, std::vector<int8_t> actions_,
                  const uint8_t pixelSize_, const sf::Color textColor_, const sf::Color backgroundColor_,
                  const sf::Color cursorColor_) : backgroundColor(backgroundColor_), cursorColor(cursorColor_),
                                                  actions(std::move(actions_)) {
        window = window_;

        lineHeight = static_cast<int16_t>(pixelSize_ * 9);

        backgroundAndCursorVertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);
        updateBackgroundAndCursor();

        len = 0;
        for (const char parameter: parameters_) {
            if (parameter == '\n') {
                len++;
            }
        }

        text = ReadOnlyText{
            window, std::move(parameters_), 0, 0, pixelSize_, window->getSize().x, window->getSize().y, textColor_,
            sf::Color::Transparent, false
        };
    }

    ~Menu() = default;

    int play() {
        int returnedValue = -1;
        while (window->isOpen()) {
            while (const std::optional event = window->pollEvent()) {
                if (event->is<sf::Event::Closed>()) {
                    window->close();
                    std::cout << "Window closed\n";
                    return -2;
                }
                if (const auto *resized = event->getIf<sf::Event::Resized>()) {
                    std::cout << "New width: " << window->getSize().x << '\n'
                            << "New height: " << window->getSize().y << '\n';
                    // update the view to the new size of the window
                    sf::FloatRect visibleArea({0.f, 0.f}, sf::Vector2f(resized->size));
                    window->setView(sf::View(visibleArea));

                    manageResizedEvent();
                } else if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                        window->close();
                        std::cout << "Window closed\n";
                        return -2;
                    }
                    returnedValue = manageKey(keyPressed->scancode);
                }
            }
            if (returnedValue != -1) {
                break;
            }

            window->clear();

            draw();

            window->display();
        }
        return returnedValue;
    }

    int manageKey(const sf::Keyboard::Scancode k) {
        const int16_t oldCursor = cursor;
        if (k == sf::Keyboard::Scancode::Right || k == sf::Keyboard::Scancode::Down) {
            cursor++;
            if (cursor >= len) {
                cursor = 0;
            }
        } else if (k == sf::Keyboard::Scancode::Left || k == sf::Keyboard::Scancode::Up) {
            cursor--;
            if (cursor < 0) {
                cursor = static_cast<int16_t>(len - 1);
            }
        } else if (k == sf::Keyboard::Scancode::Enter) {
            return actions[cursor];
        }

        if (oldCursor != cursor) {
            updateBackgroundAndCursor();
        }

        return -1;
    }

    void draw() const {
        window->draw(backgroundAndCursorVertexArray);
        text.draw();
    }
};


int main() {
    sf::RenderWindow window;
    window.create(sf::VideoMode({800, 700}), "car_plus_plus", sf::Style::Default, sf::State::Windowed);
    std::cout << "The window was created successfully\n";
    window.setFramerateLimit(10);

    std::ifstream input("tastatura.txt");
    std::string s, tmp;
    char c;
    while (input >> tmp) {
        s += tmp;
        c = static_cast<char>(input.get());
        if (c != -1) {
            s += " ";
            s[s.size() - 1] = c;
        }
    }
    input.close();
    EditableText t(&window, s, 0, 0, 2, window.getSize().x, window.getSize().y, sf::Color::Black, sf::Color::White,
                   sf::Color::Green);
    std::cout << t;

    auto *g = new Greet(&window, "Welcome to car++!");
    std::cout << *g;
    g->play();
    delete g;

    std::string parameters = "MAIN MENU\nsettings\nnew file\nopen from disk\nAI mode\ndemo text editor";
    std::vector<int8_t> actions = {1, 2, 3, 4, 5};
    int scene = 0;
    while (scene != -2) {
        if (scene == 0) {
            auto *m = new Menu(&window, parameters, actions, 3, sf::Color::Black, sf::Color::White, sf::Color::Green);
            std::cout << *m;
            scene = m->play();
            delete m;
        } else if (scene == 1) {
            auto *m = new Greet(&window, "settings");
            std::cout << *m;
            m->play();
            scene = 0;
            delete m;
        } else if (scene == 2) {
            auto *m = new Greet(&window, "new file created");
            std::cout << *m;
            m->play();
            scene = 0;
            delete m;
        } else if (scene == 3) {
            auto *m = new Greet(&window, "file opened from disk");
            std::cout << *m;
            m->play();
            scene = 0;
            delete m;
        } else if (scene == 4) {
            auto *m = new Greet(&window, "malware successfully installed");
            std::cout << *m;
            m->play();
            delete m;
            return 0;
        } else if (scene == 5) {
            scene = -2;
        }
    }


    bool ctrl = false;
    bool shift = false;
    auto k = sf::Keyboard::Scan::Unknown;
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                std::cout << "Window closed\n";
            } else if (const auto *resized = event->getIf<sf::Event::Resized>()) {
                std::cout << "New width: " << window.getSize().x << '\n'
                        << "New height: " << window.getSize().y << '\n';
                // update the view to the new size of the window
                sf::FloatRect visibleArea({0.f, 0.f}, sf::Vector2f(resized->size));
                window.setView(sf::View(visibleArea));

                t.manageResizedEvent();
            } else if (const auto *textEntered = event->getIf<sf::Event::TextEntered>()) {
                t.manageEnteredText(textEntered);
            } else if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                    window.close();
                    std::cout << "Window closed\n";
                } else if (keyPressed->scancode == sf::Keyboard::Scancode::LControl || keyPressed->scancode ==
                           sf::Keyboard::Scancode::RControl) {
                    ctrl = true;
                } else if (keyPressed->scancode == sf::Keyboard::Scancode::LShift || keyPressed->scancode ==
                           sf::Keyboard::Scancode::RShift) {
                    shift = true;
                } else {
                    k = keyPressed->scancode;
                }
            } else if (const auto *keyReleased = event->getIf<sf::Event::KeyReleased>()) {
                if (keyReleased->scancode == sf::Keyboard::Scancode::LControl || keyReleased->scancode ==
                    sf::Keyboard::Scancode::RControl) {
                    ctrl = false;
                } else if (keyReleased->scancode == sf::Keyboard::Scancode::LShift || keyReleased->scancode ==
                           sf::Keyboard::Scancode::RShift) {
                    shift = false;
                }
            }
        }

        if (k != sf::Keyboard::Scan::Unknown) {
            t.manageKey(k, ctrl, shift);
            k = sf::Keyboard::Scan::Unknown; // very important to avoid repeated calls of manageKey()
        }

        window.clear();

        t.draw();

        window.display();
    }

    std::cout << "The program finished successfully\n";
    return 0;
}
