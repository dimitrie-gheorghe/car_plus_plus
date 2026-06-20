//
// Created by dimitrie on 20.06.2026.
//

#ifndef OOP_APPEXCEPTIONS_H
#define OOP_APPEXCEPTIONS_H

#include <filesystem>
#include <ranges>
#include <string>
#include <stdexcept>

class AppError : public std::runtime_error {
public:
    explicit AppError(const std::string &msg);

    friend std::ostream &operator<<(std::ostream &os, const AppError &err);
};

class ConfigurationError : public AppError {
public:
    ConfigurationError();

    friend std::ostream &operator<<(std::ostream &os, const ConfigurationError &err);
};

class AssetLoadError : public AppError {
public:
    AssetLoadError();

    friend std::ostream &operator<<(std::ostream &os, const AssetLoadError &err);
};

class SceneInitializationError : public AppError {
public:
    SceneInitializationError();

    friend std::ostream &operator<<(std::ostream &os, const SceneInitializationError &err);
};

#endif //OOP_APPEXCEPTIONS_H