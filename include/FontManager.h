//
// Created by dimitrie on 20.06.2026.
//

#ifndef OOP_FONTMANAGER_H
#define OOP_FONTMANAGER_H

#include <filesystem>

class FontManager {
    uint8_t ascii[8 * 95]{};

    FontManager();

public:
    friend std::ostream &operator<<(std::ostream &os, const FontManager &);

    // static const FontManager &getInstance();
    //
    // [[nodiscard]] uint8_t getByte(size_t index) const;

    // The ultimate solution: define the functions inside the header
    static const FontManager &getInstance() {
        // Actually I use it (look at VertexArrayUtility::insertChar() in VertexArrayUtility.cpp) but the cppCheck gives me a false positive
        static FontManager instance;
        return instance;
    }

    [[nodiscard]] uint8_t getByte(const size_t index) const {
        // Actually I use it (look at VertexArrayUtility::insertChar() in VertexArrayUtility.cpp) but the cppCheck gives me a false positive
        return ascii[index];
    }
};

#endif //OOP_FONTMANAGER_H