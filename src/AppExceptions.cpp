//
// Created by dimitrie on 20.06.2026.
//

#include "../include/AppExceptions.h"
#include <ostream>
#include <ranges>
#include <string>
#include <sstream>
#include <stdexcept>

AppError::AppError(const std::string &msg) : std::runtime_error(msg) {
}

std::ostream &operator<<(std::ostream &os, const AppError &err) {
    os << "AppError: " << err.what();
    return os;
}

ConfigurationError::ConfigurationError() : AppError("Error: Configuration file is malformed or missing.") {
}

std::ostream &operator<<(std::ostream &os, const ConfigurationError &err) {
    os << "ConfigurationError: " << err.what();
    return os;
}


AssetLoadError::AssetLoadError() : AppError("Error: Required assets could not be loaded.") {
}

std::ostream &operator<<(std::ostream &os, const AssetLoadError &err) {
    os << "AssetLoadError: " << err.what();
    return os;
}

SceneInitializationError::SceneInitializationError() : AppError("Error: Failed to initialize a required scene.") {
}

std::ostream &operator<<(std::ostream &os, const SceneInitializationError &err) {
    os << "SceneInitializationError: " << err.what();
    return os;
}
