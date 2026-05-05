#include "ResourceManager.h"
#include <SFML/Graphics/Font.hpp>
#include <string>
#include <iostream>
#include <cstddef>
#include <cassert>
#include <array>

namespace ResourceManager {
    namespace {
        constexpr std::array<std::string_view, Count> fontPaths {
            "assets/fonts/ARCADE.TTF"
        };
        
        std::array<sf::Font, Count> loadFonts() {
            std::array<sf::Font, Count> fonts {};
            for (std::size_t i {0}; i < fonts.size(); i++) {
                if (!fonts[i].openFromFile(fontPaths[i])) {
                    std::cerr << "ERROR: couldn't load the font: " << fontPaths[i] << '\n';
                }
            }
            return fonts;
        }
    }

    const sf::Font& getFont(Fonts fontEnum) {
        static std::array<sf::Font, Count> fonts {loadFonts()};
        assert(fontEnum < Count);
        return fonts[fontEnum];
    }
}