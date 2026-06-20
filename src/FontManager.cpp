//
// Created by dimitrie on 20.06.2026.
//

#include "FontManager.h"
#include <filesystem>
#include <fstream>
#include "../ext/portable-file-dialogs.h"
#include "../include/AppExceptions.h"

FontManager::FontManager() {
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

const FontManager &FontManager::getInstance() {
    static FontManager instance;
    return instance;
}

[[nodiscard]] uint8_t FontManager::getByte(const size_t index) const {
    return ascii[index];
}

std::ostream &operator<<(std::ostream &os, const FontManager &) {
    os << "FontManager (Singleton Instance holding custom ASCII definitions)";
    return os;
}
