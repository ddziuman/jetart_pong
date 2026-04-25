#include "WindowManager.h"
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowEnums.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <string_view>
#include <optional>

WindowManager::WindowManager()
: WindowManager("Pong", 60)
{}

WindowManager::WindowManager(std::string_view title, unsigned int frameLimit)
: m_videoMode {sf::VideoMode::getDesktopMode()}
, m_window {m_videoMode, title, sf::State::Fullscreen}
, m_renderSize {m_window.getSize()}
, m_center {m_renderSize.x / 2, m_renderSize.y / 2}
{
        m_window.setFramerateLimit(frameLimit);
}

float WindowManager::normalizePct(float pct) {
    if (pct < -100 || pct > 100) {
        return 0.5;
    }
    return (pct < 0 ? ((pct + 100)) : pct) / 100;
}

sf::RenderWindow& WindowManager::getWindow() {
    return m_window;
}

float WindowManager::relX(float pct) {// +(1-100)% from the left/top / -(1-100)% if from the right/bottom
    return m_renderSize.x * normalizePct(pct);
}

float WindowManager::relY(float pct) {
    return m_renderSize.y * normalizePct(pct);
}

float WindowManager::offsetX(float offset) {
    return offset < 0 ? m_renderSize.x + offset : offset;
}

float WindowManager::offsetY(float offset) {
    return offset < 0 ? m_renderSize.y + offset : offset;
}

float WindowManager::centerOffsetX(float offset) {
    return m_center.x + offset;
}

float WindowManager::centerOffsetY(float offset) {
    return m_center.y + offset;
}

void WindowManager::handleEvents() {
    while (const std::optional event {m_window.pollEvent()}) {
        if (event->is<sf::Event::Closed>() ||
            event->is<sf::Event::KeyPressed>() &&
            event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape)
            m_window.close();
        
        // other events
    }
}