//
// Created by dimitrie on 20.06.2026.
//

#include "../include/FontManager.h"
#include <filesystem>
#include <fstream>
#include "../include/AppExceptions.h"

FontManager::FontManager() {
    const std::filesystem::path fontPath = std::filesystem::current_path() / "assets" / "font.txt";

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

[[nodiscard]] uint8_t FontManager::getByte(const size_t index) const {
    // Actually I use it (look at VertexArrayUtility::insertChar() in VertexArrayUtility.cpp) but the cppCheck gives me a false positive
    return ascii[index];
}

std::ostream &operator<<(std::ostream &os, const FontManager &fm) {
    // This was the ultimate solution to silence the cppCheck's unused errors
    // Actually I use these functions (look at VertexArrayUtility::insertChar() in VertexArrayUtility.cpp)
    // Dummy calls to silence cppcheck's isolated file scanner.
    // The compiler will optimize these away entirely, but cppcheck will register them as "used".
    //(void)fm.getByte(0);

    os << "FontManager (Singleton Instance holding custom ASCII definitions)";
    return os;
}
