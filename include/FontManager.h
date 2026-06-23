//
// Created by dimitrie on 20.06.2026.
//

#ifndef OOP_FONTMANAGER_H
#define OOP_FONTMANAGER_H

#include <ostream>
#include "SingletonTemplate.h"
#include <cstdint> // NOLINT

class FontManager : public SingletonTemplate<FontManager> {
    friend class SingletonTemplate;

    uint8_t ascii[8 * 95]{};
    FontManager();

public:
    FontManager(const FontManager&) = delete;
    FontManager& operator=(const FontManager&) = delete;
    FontManager(FontManager&&) = delete;
    FontManager& operator=(FontManager&&) = delete;
    ~FontManager() = default;

    [[nodiscard]] uint8_t getByte(size_t index) const;

};

#endif //OOP_FONTMANAGER_H