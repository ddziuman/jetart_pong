#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H
#include <SFML/Graphics/Font.hpp>

namespace ResourceManager {
    enum Fonts {
        Arcade,
        Count
    };

    const sf::Font& getFont(Fonts fontEnum);
};

#endif