#include "Engine.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

void Engine::init() {
    sf::Vector2f playerSize {m_wm.relX(1), m_wm.relY(10)};
    sf::Vector2f playerPos {m_wm.relX(20), m_wm.centerOffsetY(-(playerSize.y / 2))};
    m_player = PlayerPaddle{playerSize, playerPos, {0, 10}, {0, 1000}, sf::Color::White};
} 


bool Engine::run() {
    init();

    auto& window {m_wm.getWindow()};
    while (window.isOpen()) {
        m_timer.beginFrame();
        m_wm.handleEvents();
        m_player.update(m_timer.deltaTime());

        // draw functions
        window.clear();
        window.draw(m_player.getPlayerRect());

        window.display();
    }
    return 0;
}