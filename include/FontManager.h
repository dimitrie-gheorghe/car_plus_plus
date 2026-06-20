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

    static const FontManager &getInstance();

    [[nodiscard]] uint8_t getByte(size_t index) const;
};

#endif //OOP_FONTMANAGER_H