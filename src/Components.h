#ifndef COMPONENTS_H
#define COMPONENTS_H
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

struct Component {
    bool exists {false};
};

struct CTransform : public Component {
    sf::Vector2f m_position {0, 0};
    sf::Vector2f m_velocity {0, 0};
};

struct CShapeCircle : public Component {
    sf::CircleShape shape {};
};

struct CShapeRectangle : public Component {
    sf::RectangleShape shape {};
};

struct CHealth : public Component {
    int remainingHp {0};
};

struct CColliderBox : public Component {

};

struct CColliderCircle : public Component {

};

struct CInput : public Component {
    // something like a map of keyboard buttons from 'sf::' and moving states (enum?)?
};

struct CAI : public Component {
    // some way of having a bot AI playing
};

#endif