#ifndef ENGINE_H
#define ENGINE_H
#include "WindowManager.h"
#include "ActorManager.h"
#include "ResourceManager.h"
#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Engine {
private:
    WindowManager m_wm {};
    ActorManager m_am {};
    sf::Clock m_deltaClock {};
    sf::Clock m_startPauseClock {};
    sf::Clock m_nextBallClock {};
    bool m_paused {true};
    bool m_respawnSide {false};
    
public:
    bool run();
    void init();

    void movement(float dt);
    void moveActors(float dt, Actor::Tag actorTag);

    void input();
    void aiScript(float dt);

    void render(sf::RenderWindow& target);
    void renderPlayers(sf::RenderWindow& target, Actor::Tag playerTag);
    void renderBall(sf::RenderWindow& target);
    void renderNet(sf::RenderWindow& target);

    void collisions();
    void wallBallCollision();
    void wallPlayerCollision(Actor::Tag playerTag);
    void ballPlayerCollision(Actor::Tag playerTag);

    void incrementPlayerScore(Actor::Tag playerTag);
    
    void spawnBall(bool side);
    void respawnBall(bool side, float seconds);
    void spawnPlayer();
    void spawnBot();
    void spawnPlayingNet();

};

#endif