#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <string_view>

class WindowManager { // WindowManager is abstraction layer over SFML Window, we can adapt to any other graphics library because of it
                      // I want it to return me %-wise coordinates, offseted coordinates, centralized, etc.
private:
    sf::VideoMode m_videoMode {};
    sf::RenderWindow m_window {};
    sf::Vector2f m_renderSize {};
    sf::Vector2f m_center {};

    float normalizePct(float pct);

public:
    WindowManager();
    WindowManager(std::string_view title, unsigned int frameLimit);
    sf::RenderWindow& getWindow();

    float relX(float pct);              // +(1-100)% if from the left / -(1-100)% if from the right
    float relY(float pct);              // +(1-100)% if from the top / -(1-100)% if from the bottom
    float offsetX(float offset);        // +offset (0 + offset) from left, -offset (max + offset) from the right
    float offsetY(float offset);        // +offset (0 + offset) from the top, -offset (max + offset) from the bottom
    float centerOffsetX(float offset);  // (same offsetting, but relative to center)
    float centerOffsetY(float offset);  

    void handleEvents();
};

#endif