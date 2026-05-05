#ifndef COMPONENTS_H
#define COMPONENTS_H
#include "ResourceManager.h"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/System/Angle.hpp>
#include <cstddef>
#include <string_view>
#include <string>
#include <iostream>
#include <cmath>

class Component {
public:
    bool exists {false};
};

class CTransform : public Component {
public: 
    sf::Vector2f position {};
    sf::Vector2f velocity {};
    float maxSpeed {};

    CTransform() = default;
    CTransform(const sf::Vector2f& initPos, float maxSpeed, const sf::Vector2f& initVel = {})
    : position {initPos}
    , velocity {initVel}
    , maxSpeed {maxSpeed}
    {}
    CTransform(const sf::Vector2f& initPos, float maxSpeed, float speed, const sf::Angle& angle = {})
    : position {initPos}
    , velocity {std::cosf(angle.asRadians()) * speed, std::sinf(angle.asRadians()) * speed}
    , maxSpeed {maxSpeed}
    {}
};

class CShapeCircle : public Component {
public:
    sf::CircleShape shape {};

    CShapeCircle() = default;
    CShapeCircle(int radius, std::size_t pointCount, const sf::Color& fill)
    : shape {radius, pointCount}
    {
        shape.setOrigin({radius, radius}); // to easen up collision math for the ball
        shape.setFillColor(fill);
    }
};

class CShapeRectangle : public Component {
public:
    sf::RectangleShape shape {};

    CShapeRectangle() = default;
    CShapeRectangle(float sizeX, float sizeY, const sf::Color& fill)
    : shape {{sizeX, sizeY}}
    {
        shape.setFillColor(fill);
    }
};

class CHealth : public Component {
public:
    int remainingHp {0};

    CHealth() = default;
    explicit CHealth(int totalHp)
    : remainingHp {totalHp}
    {}
};

class CColliderBox : public Component {
public:
    // Axis-Aligned Bounding Box (AABB)
    sf::Vector2f bboxPosition {};
    sf::Vector2f bboxSize {};

    CColliderBox() = default;
    CColliderBox(const sf::Vector2f& position, const sf::Vector2f& size)
    : bboxPosition {position}
    , bboxSize {size}
    {}
};

class CColliderCircle : public Component {
public:
    sf::Vector2f bcircleCenter {};
    int bcircleRadius {};

    CColliderCircle() = default;
    CColliderCircle(const sf::Vector2f& center, int radius)
    : bcircleCenter {center}
    , bcircleRadius {radius}
    {}
};

class CInput : public Component {
    // something like a map of keyboard buttons from 'sf::' and moving states (enum?)?
public:
    bool up {false};
    bool down {false};
};

class CAI : public Component {
public:
    float deadZoneY {};
    float decisionInterval {}; // 'difficulty'
    float thinkingTime {};
    float cachedTargetY {};

    CAI() = default;
    CAI(float decisionIntervalSec, float deadZoneY)
    : deadZoneY {deadZoneY}
    , decisionInterval {decisionIntervalSec}
    {}
};

class CTextScore : public Component {
public:
    int score {0};
    sf::Text text;

    CTextScore()
    : score {0}
    , text {ResourceManager::getFont(ResourceManager::Arcade)}
    {
        setup(sf::Vector2f{0, 0}, 60, sf::Color::White);
    }

    explicit CTextScore(int initScore, ResourceManager::Fonts fontId, const sf::Vector2f& position, unsigned int fontSize, const sf::Color& fill)
    : score {initScore}
    , text {ResourceManager::getFont(fontId)}
    {
        setup(position, fontSize, fill);
    }

private:
    void setup(const sf::Vector2f& position, unsigned int fontSize, const sf::Color& fill) {
        text.setOrigin(text.getLocalBounds().position);
        text.setPosition(position);
        text.setString(std::to_string(score));
        text.setCharacterSize(fontSize);
        text.setFillColor(fill);
        text.setOutlineColor(sf::Color::White);
        text.setStyle(sf::Text::Bold);
    }
};

#endif