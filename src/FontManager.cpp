//
// Created by dimitrie on 20.06.2026.
//

#include "../include/FontManager.h"
#include <filesystem>
#include <fstream>
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

// [[maybe_unused]] const FontManager &FontManager::getInstance() {
//     // Actually I use it (look at VertexArrayUtility::insertChar() in VertexArrayUtility.cpp) but the cppCheck gives me a false positive
//     static FontManager instance;
//     return instance;
// }
//
// [[maybe_unused]] [[nodiscard]] uint8_t FontManager::getByte(const size_t index) const {
//     // Actually I use it (look at VertexArrayUtility::insertChar() in VertexArrayUtility.cpp) but the cppCheck gives me a false positive
//     return ascii[index];
// }

std::ostream &operator<<(std::ostream &os, const FontManager &) {
    os << "FontManager (Singleton Instance holding custom ASCII definitions)";
    return os;
}
