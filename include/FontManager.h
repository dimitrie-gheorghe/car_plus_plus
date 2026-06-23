//
// Created by dimitrie on 20.06.2026.
//

#ifndef OOP_FONTMANAGER_H
#define OOP_FONTMANAGER_H

#include <ostream>

#include "SingletonTemplate.h"

class FontManager : public SingletonTemplate<FontManager> {
    friend class SingletonTemplate;

    uint8_t ascii[8 * 95]{};
    FontManager();

public:
    ~FontManager() = default;

    [[nodiscard]] uint8_t getByte(size_t index) const;
};

#endif //OOP_FONTMANAGER_H