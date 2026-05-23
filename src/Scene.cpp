//
// Created by dimitrie on 23.05.2026.
//

#include <SFML/Graphics.hpp>
#include <iostream>
#include "../include/Scene.h"

int Scene::play() {
    int returnedValue = 0;
    bool shouldExit = false;
    while (windowUsedForMainLoop->isOpen()) {
        while (const std::optional event = windowUsedForMainLoop->pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                shouldExit = true;
            } else if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                    shouldExit = true;
                }
            } else if (const auto *resized = event->getIf<sf::Event::Resized>()) {
                std::cout << "New width: " << windowUsedForMainLoop->getSize().x << '\n'
                        << "New height: " << windowUsedForMainLoop->getSize().y << '\n';
                // update the view to the new size of the window
                sf::FloatRect visibleArea({0.f, 0.f}, sf::Vector2f(resized->size));
                windowUsedForMainLoop->setView(sf::View(visibleArea));
            }
            eventForDerivedClasses = event;
            returnedValue = manageOtherEvents();
        }
        if (shouldExit) {
            windowUsedForMainLoop->close();
            std::cout << "Window closed\n";
            return -1;
        }


        windowUsedForMainLoop->clear();

        if (returnedValue == 0) {
            returnedValue = draw();
        }

        windowUsedForMainLoop->display();

        if (returnedValue != 0) {
            break;
        }
    }
    return returnedValue;
}