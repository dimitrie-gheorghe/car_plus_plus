#include <iostream>
#include <fstream>
#include <thread>

#include <SFML/Graphics.hpp>

#include "include/AppManager.h"

// class SceneManager {
//     Scene* s;
// };

int main() {
    constexpr sf::Color textColor = sf::Color::Black;
    constexpr sf::Color backgroundColor = sf::Color::White;
    constexpr sf::Color cursorColor = sf::Color::Green;

    sf::RenderWindow window;
    window.create(sf::VideoMode({800, 700}), "car_plus_plus", sf::Style::Default, sf::State::Windowed);
    std::cout << "The window was created successfully\n";
    window.setFramerateLimit(10);


    std::ifstream input("tastatura.txt");
    std::string s, tmp;
    char c;
    while (input >> tmp) {
        s += tmp;
        c = static_cast<char>(input.get());
        if (c != -1) {
            s += " ";
            s[s.size() - 1] = c;
        }
    }
    input.close();

    // auto *g = new Greet(&window, "Welcome to car++!");
    // std::cout << *g;
    // g->play();
    // delete g;

    std::string parameters = "MAIN MENU\nsettings\nnew file\nopen from disk\nAI mode\ndemo text editor";
    std::vector<int8_t> actions = {1, 2, 3, 4, 5};
    int scene = 1;
    while (scene != -1) {
        if (scene == 1) {
            auto *m = new Menu(&window, parameters, actions, 3, textColor, backgroundColor, cursorColor);
            std::cout << *m;
            scene = m->play();
            delete m;
        } else if (scene == 2) {
            auto *m = new TilePanel(&window, 10, 10, 0, 0, 500, 250, textColor, backgroundColor, cursorColor);
            m->play();
            scene = 0;
            delete m;
        } else if (scene == 3) {
            auto *m = new Greet(&window, "new file created");
            std::cout << *m;
            m->play();
            scene = 0;
            delete m;
        } else if (scene == 4) {
            auto *m = new Greet(&window, "file opened from disk");
            std::cout << *m;
            m->play();
            scene = 0;
            delete m;
        } else if (scene == 5) {
            auto *m = new Greet(&window, "malware successfully installed");
            std::cout << *m;
            m->play();
            delete m;
            return 0;
        } else if (scene == 6) {
            auto *m = new EditableText(&window, s, 0, 0, 2, window.getSize().x, window.getSize().y, textColor,
                                       backgroundColor,
                                       cursorColor);
            std::cout << *m;
            m->play();
            scene = 0;
            delete m;
        }
    }

    std::cout << "The program finished successfully\n";
    return 0;
}
