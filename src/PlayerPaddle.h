#ifndef PLAYER_PADDLE_H
#define PLAYER_PADDLE_H
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

class PlayerPaddle {
private:
    sf::Vector2f m_position {};
    sf::Vector2f m_velocity {};
    sf::Vector2f m_acceleration {};
    sf::RectangleShape m_player {};

public:
    PlayerPaddle() = default;
    PlayerPaddle(const sf::Vector2f& size, const sf::Vector2f& pos, const sf::Vector2f& vel, const sf::Vector2f& accel, const sf::Color& color);
    const sf::RectangleShape& getPlayerRect() const;

    void update(float dt);
};

#endif