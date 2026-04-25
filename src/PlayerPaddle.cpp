#include "PlayerPaddle.h"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

PlayerPaddle::PlayerPaddle(const sf::Vector2f& size, const sf::Vector2f& pos, const sf::Vector2f& vel, const sf::Vector2f& accel, const sf::Color& color)
: m_position {pos}
, m_velocity {vel}
, m_acceleration {accel}
, m_player {size}
{
    m_player.setPosition(pos);
    m_player.setFillColor(color);
}

void PlayerPaddle::update(float dt) {
    m_velocity += m_acceleration * dt;
    m_position += m_velocity * dt;
    m_player.setPosition(m_position);
}

const sf::RectangleShape& PlayerPaddle::getPlayerRect() const {
    return m_player;
}