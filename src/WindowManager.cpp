#include "WindowManager.h"
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowEnums.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <string_view>
#include <optional>

WindowManager::WindowManager()
: WindowManager("Pong", 60, sf::State::Fullscreen)
{}

WindowManager::WindowManager(std::string_view title, unsigned int frameLimit, sf::State mode)
: m_videoMode {sf::VideoMode::getDesktopMode()}
, m_window {m_videoMode, title, mode}
{
        m_window.setFramerateLimit(frameLimit);
        m_window.setKeyRepeatEnabled(false);
}

sf::Vector2u WindowManager::renderSize() {
    return m_window.getSize();
}

sf::Vector2u WindowManager::center() {
    const auto size {renderSize()};
    return {size.x * 0.5, size.y * 0.5};
}

float WindowManager::normalizePct(float pct) {
    if (pct < -100 || pct > 100) {
        return 0.5;
    }
    return (pct < 0 ? ((pct + 100)) : pct) * 0.01;
}

sf::RenderWindow& WindowManager::getWindow() {
    return m_window;
}

float WindowManager::relX(float pct) {// +(1-100)% from the left/top / -(1-100)% if from the right/bottom
    return renderSize().x * normalizePct(pct);
}

float WindowManager::relY(float pct) {
    return renderSize().y * normalizePct(pct);
}

float WindowManager::offsetX(float offset) {
    return offset < 0 ? renderSize().x + offset : offset;
}

float WindowManager::offsetY(float offset) {
    return offset < 0 ? renderSize().y + offset : offset;
}

float WindowManager::centerOffsetX(float offset) {
    return center().x + offset;
}

float WindowManager::centerOffsetY(float offset) {
    return center().y + offset;
}