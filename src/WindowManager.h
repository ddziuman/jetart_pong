#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <string_view>

class WindowManager { // for %-wise coordinates, offseted coordinates, centralization, etc.
                      
private:
    sf::VideoMode m_videoMode {};
    sf::RenderWindow m_window {};

    float normalizePct(float pct);

public:
    WindowManager();
    WindowManager(std::string_view title, unsigned int frameLimit, sf::State mode);
    sf::RenderWindow& getWindow();
    sf::Vector2u renderSize();
    sf::Vector2u center();

    float relX(float pct);              // +(1-100)% if from the left / -(1-100)% if from the right
    float relY(float pct);              // +(1-100)% if from the top / -(1-100)% if from the bottom
    float offsetX(float offset);        // +offset (0 + offset) from left, -offset (max + offset) from the right
    float offsetY(float offset);        // +offset (0 + offset) from the top, -offset (max + offset) from the bottom
    float centerOffsetX(float offset);  // (same offsetting, but relative to center)
    float centerOffsetY(float offset);
};

#endif