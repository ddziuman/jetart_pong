#include "Engine.h"
#include "Actor.h"
#include "Components.h"
#include "Random.h"
#include "ResourceManager.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Window/WindowEnums.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Angle.hpp>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <string>
#include <cassert>

void Engine::init() {
    spawnPlayer();
    spawnBot();
    spawnBall(false);
    spawnPlayingNet();
    m_nextBallClock.stop();
    m_nextBallClock.reset();
} 

bool Engine::run() {
    init();
    auto& window {m_wm.getWindow()};
    m_startPauseClock.restart();
    m_deltaClock.restart();
    while (window.isOpen()) {
        float dt {m_deltaClock.restart().asSeconds()};                              // delta time
        if (m_paused && m_startPauseClock.getElapsedTime().asSeconds() >= 1.0f) {   // for starting pause
            m_paused = false;
            m_startPauseClock.stop();
        }
        respawnBall(m_respawnSide, 1.0f);
        input();                                                                    // user input events
        aiScript(dt);                                                               // bot paddle script
        if (!m_paused) {
            m_am.update();                                                          // actor manager 'garbage collection'
            movement(dt);                                                           // movement system for all actors
            collisions();                                                           // collision detection and resolve
        }
        render(window);                                                             // clear/draw/display system
    }
    return 0;
}

void Engine::movement(float dt) {
    moveActors(dt, Actor::Tag::PlayerPaddle);
    moveActors(dt, Actor::Tag::BotPaddle);
    moveActors(dt, Actor::Tag::Ball);
}

void Engine::moveActors(float dt, Actor::Tag actorTag) {
    const auto& actors {m_am.getActors(actorTag)};
    for (const auto& actor : actors) {
        auto& input {actor->getComponent<CInput>()};
        auto& ai {actor->getComponent<CAI>()};
        auto& transform {actor->getComponent<CTransform>()};
        auto& bbox {actor->getComponent<CColliderBox>()};
        auto& bcircle {actor->getComponent<CColliderCircle>()};
        if (!transform.exists) continue;

        auto& velocity {transform.velocity};
        if (input.exists && (actorTag == Actor::Tag::PlayerPaddle)) {
            velocity.y = input.up ? -transform.maxSpeed : input.down ? transform.maxSpeed : 0;
        }

        const auto deltaVelocity {transform.velocity * dt};
        transform.position += deltaVelocity;
        if (bbox.exists) {
            bbox.bboxPosition += deltaVelocity;
        }
        if (bcircle.exists) {
            bcircle.bcircleCenter += deltaVelocity;
        }
    }
}

void Engine::input() {

    auto& window {m_wm.getWindow()};
    auto& players {m_am.getActors(Actor::Tag::PlayerPaddle)};
    const auto goUp {[](auto& a) {
        CInput& input (a->template getComponent<CInput>());
        if (input.exists) {
            input.up = true;
        }
    }};
    const auto goDown {[](auto& a) {
        CInput& input (a->template getComponent<CInput>());
        if (input.exists) {
            input.down = true;
        }
    }};
    const auto stopUp {[](auto& a) {
        CInput& input (a->template getComponent<CInput>());
        if (input.exists) {
            input.up = false;
        }
    }};
    const auto stopDown {[](auto& a) {
        CInput& input (a->template getComponent<CInput>());
        if (input.exists) {
            input.down = false;
        }
    }};
    while (const std::optional event {window.pollEvent()}) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        if (const auto* keyPressed {event->getIf<sf::Event::KeyPressed>()}) {
            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                window.close();
            }
            if (keyPressed->scancode == sf::Keyboard::Scancode::W) {
                std::cout << "W (pressed)\n";
                std::ranges::for_each(players, goUp);
            }
            if (keyPressed->scancode == sf::Keyboard::Scancode::S) {
                std::ranges::for_each(players, goDown);
            }
        }
        if (const auto& keyReleased {event->getIf<sf::Event::KeyReleased>()}) {
            if (keyReleased->scancode == sf::Keyboard::Scancode::W) {
                std::cout << "W (released)\n";
                std::ranges::for_each(players, stopUp);
            }
            if (keyReleased->scancode == sf::Keyboard::Scancode::S) {
                std::ranges::for_each(players, stopDown);
            }
        }
    }
}

void Engine::aiScript(float dt) {
    const auto& bots {m_am.getActors(Actor::Tag::BotPaddle)};
    for (const auto& bot : bots) {
        auto& transform {bot->getComponent<CTransform>()};
        const auto& shape {bot->getComponent<CShapeRectangle>()};
        auto& cai {bot->getComponent<CAI>()};
        if (!cai.exists || !transform.exists) continue;

        cai.thinkingTime += dt;
        if (cai.thinkingTime >= cai.decisionInterval) {
            const auto& balls {m_am.getActors(Actor::Tag::Ball)};
            for (auto& ball : balls) {
                const auto& ballTransform {ball->getComponent<CTransform>()};
                if (!ballTransform.exists) continue;
                cai.cachedTargetY = ballTransform.position.y;
            }
            cai.thinkingTime = 0.0f;
        }
        if (!shape.exists) continue;
        float paddleCenterY = transform.position.y + (shape.shape.getSize().y * 0.5f);
        float targetDistance {cai.cachedTargetY - paddleCenterY};
        if (std::abs(targetDistance) > cai.deadZoneY)
            transform.velocity.y = targetDistance > 0 ? transform.maxSpeed : -transform.maxSpeed;
        else
            transform.velocity.y = 0;
    }
}

void Engine::render(sf::RenderWindow& target) {
    target.clear();
    renderPlayers(target, Actor::Tag::PlayerPaddle);
    renderPlayers(target, Actor::Tag::BotPaddle);
    renderBall(target);
    renderNet(target);
    target.display();
}

void Engine::renderPlayers(sf::RenderWindow& target, Actor::Tag playerTag) {
    assert(playerTag == Actor::Tag::PlayerPaddle || playerTag == Actor::Tag::BotPaddle);
    auto& players {m_am.getActors(playerTag)};
    for (const auto& player : players) {
        const auto& transform {player->getComponent<CTransform>()};
        auto& shape {player->getComponent<CShapeRectangle>()};
        auto& textScore {player->getComponent<CTextScore>()};
        if (!transform.exists || !shape.exists) continue;
        shape.shape.setPosition(transform.position);
        target.draw(shape.shape);
        if (textScore.exists) {
            textScore.text.setString(std::to_string(textScore.score));
            target.draw(textScore.text);  
        }
    }
}

void Engine::renderBall(sf::RenderWindow& target) {
    auto& balls {m_am.getActors(Actor::Tag::Ball)};
    for (const auto& ball : balls) {
        const auto& transform {ball->getComponent<CTransform>()};
        auto& circleShape {ball->getComponent<CShapeCircle>()};
        if (!transform.exists || !circleShape.exists) continue;
        circleShape.shape.setPosition(transform.position);
        target.draw(circleShape.shape);
    }
}

void Engine::renderNet(sf::RenderWindow& target) {
    auto& nets {m_am.getActors(Actor::Tag::Net)};
    for (const auto& net : nets) {
        const auto& transform {net->getComponent<CTransform>()};
        auto& rectShape {net->getComponent<CShapeRectangle>()};
        if (!transform.exists || !rectShape.exists) continue;
        target.draw(rectShape.shape);
    }
}

void Engine::collisions() {
    wallBallCollision();
    wallPlayerCollision(Actor::Tag::PlayerPaddle);
    wallPlayerCollision(Actor::Tag::BotPaddle);
    ballPlayerCollision(Actor::Tag::PlayerPaddle);
    ballPlayerCollision(Actor::Tag::BotPaddle);
}

void Engine::wallBallCollision() {
    const auto& balls {m_am.getActors(Actor::Tag::Ball)};
    const auto& renderSize {m_wm.renderSize()};
    std::cout << renderSize.y << '\n';
    for (const auto& ball : balls) {
        auto& transform {ball->getComponent<CTransform>()};
        auto& bcircle {ball->getComponent<CColliderCircle>()};
        if (!transform.exists || !bcircle.exists) return;
        auto& boundingCenter {bcircle.bcircleCenter};
        int boundingRadius {bcircle.bcircleRadius};
        // TOP
        if (boundingCenter.y - boundingRadius <= 0) {
            transform.position.y = boundingRadius;
            boundingCenter.y = transform.position.y;
            transform.velocity.y *= -1;
        }
        // BOTTOM
        if (boundingCenter.y + boundingRadius >= renderSize.y) {
            transform.position.y = renderSize.y - boundingRadius;
            boundingCenter.y = transform.position.y;
            transform.velocity.y *= -1;
        }
        // LEFT
        if (boundingCenter.x - boundingRadius <= 0) {
            ball->destroy();
            incrementPlayerScore(Actor::Tag::BotPaddle);
            m_respawnSide = false;
            m_nextBallClock.start();
        }
        // RIGHT
        if (boundingCenter.x + boundingRadius >= renderSize.x) {
            ball->destroy();
            incrementPlayerScore(Actor::Tag::PlayerPaddle);
            m_respawnSide = true;
            m_nextBallClock.start();
        }
    }
}

void Engine::wallPlayerCollision(Actor::Tag playerTag) {
    const auto& players {m_am.getActors(playerTag)};
    const auto& renderSize {m_wm.renderSize()};
    for (const auto& player : players) {
        auto& transform {player->getComponent<CTransform>()};
        auto& bbox {player->getComponent<CColliderBox>()};
        if (!transform.exists || !bbox.exists) return;
        auto& bboxPos {bbox.bboxPosition};
        if (bboxPos.y <= 0) {
            bboxPos.y = 0;
            transform.position.y = bboxPos.y;
        }
        if (bboxPos.y + bbox.bboxSize.y >= renderSize.y) {
            bboxPos.y = renderSize.y - bbox.bboxSize.y;
            transform.position.y = bboxPos.y;
        }
    }
}

void Engine::ballPlayerCollision(Actor::Tag playerTag) {
    const auto& players {m_am.getActors(playerTag)};
    const auto& balls {m_am.getActors(Actor::Tag::Ball)};
    for (const auto& ball : balls) {
        auto& bcircle {ball->getComponent<CColliderCircle>()};
        auto& ballTransform {ball->getComponent<CTransform>()};
        if (!ballTransform.exists || !bcircle.exists) continue;
        for (const auto& player : players) {
            const auto& bbox {player->getComponent<CColliderBox>()};
            auto& playerTransform {player->getComponent<CTransform>()};
            if (!playerTransform.exists || !bbox.exists) continue;
    
            // detection
            auto& c {bcircle.bcircleCenter};
            auto& r {bbox.bboxPosition};
            const auto& rs {bbox.bboxSize};
            sf::Vector2f cep {c.x, c.y}; // closest edge point
            cep.x = std::clamp(cep.x, r.x, r.x + rs.x);
            cep.y = std::clamp(cep.y, r.y, r.y + rs.y);
            const sf::Vector2f distVec {c.x - cep.x, c.y - cep.y};
            const float dist {distVec.length()};
            if (dist > bcircle.bcircleRadius) continue;

            // (resolve based on where the ball hit the paddle:)
            const float halfH {rs.y * 0.5f};
            const float paddleMiddleY {r.y + halfH};
            float cepMidDiffY {cep.y - paddleMiddleY};
            cepMidDiffY /= halfH;
            cepMidDiffY = std::clamp(cepMidDiffY, -1.0f, 1.0f);
            const sf::Angle maxAngle {sf::degrees(55.0f)};
            const sf::Angle bounceAngle {cepMidDiffY * maxAngle};
            const float speed {ballTransform.velocity.length()};
            const int oppositeDirX {(ballTransform.velocity.x > 0) ? -1 : 1};
            ballTransform.velocity.x = oppositeDirX * speed * std::cosf(bounceAngle.asRadians());
            ballTransform.velocity.y = speed * std::sinf(bounceAngle.asRadians());

            // push out position to avoid jittery bounces (probably buggy)
            sf::Vector2f normal {dist <= 0.001f ? -ballTransform.velocity.normalized() : distVec / dist};
            sf::Vector2f pushout {(bcircle.bcircleRadius - dist) * normal};
            ballTransform.position += pushout;
            c += pushout;
        }
    }
}

void Engine::incrementPlayerScore(Actor::Tag playerTag) {
    const auto& players {m_am.getActors(playerTag)};
    for (const auto& player : players) {
        auto& textScore {player->getComponent<CTextScore>()};
        if (!textScore.exists) return;
        textScore.score++;
        textScore.text.setString(std::to_string(textScore.score));
    }
}

void Engine::spawnBall(bool side) {
    auto ball {m_am.createActor(Actor::Tag::Ball)};
    auto radius {ball->addComponent<CShapeCircle>(m_wm.relY(0.8), 10, sf::Color::White).shape.getRadius()};
    const float maxSpeed {m_wm.relY(50)};
    const auto angle {sf::degrees(135)};
    const sf::Vector2f velocity {std::cosf(angle.asRadians()) * maxSpeed, std::sinf(angle.asRadians()) * maxSpeed};
    auto position {ball->addComponent<CTransform>(
        sf::Vector2f{m_wm.center().x, Random::get(radius * 2, m_wm.offsetY(-radius * 2))}, 
        maxSpeed,
        velocity).position
    };
    ball->addComponent<CColliderCircle>(position, radius);
}

void Engine::respawnBall(bool side, float seconds) {
    if (m_nextBallClock.isRunning() && m_nextBallClock.getElapsedTime().asSeconds() >= seconds) {
        m_nextBallClock.stop();
        m_nextBallClock.reset();
        spawnBall(side);
    }
}

void Engine::spawnPlayer() {
    auto player {m_am.createActor(Actor::Tag::PlayerPaddle)};
    auto shapeSize {player->addComponent<CShapeRectangle>(m_wm.relX(1), m_wm.relY(10), sf::Color::Green).shape.getSize()};
    const auto& transform {player->addComponent<CTransform>(
        sf::Vector2f{m_wm.relX(20), m_wm.centerOffsetY(-(shapeSize.y * 0.5))},
        m_wm.relY(100),
        sf::Vector2f{0, 0})
    };
    player->addComponent<CColliderBox>(transform.position, shapeSize);
    player->addComponent<CInput>();
    player->addComponent<CTextScore>(
        0, 
        ResourceManager::Arcade, 
        sf::Vector2f{m_wm.relX(25), m_wm.relY(1)},
        60,
        sf::Color::Green
    );
}

void Engine::spawnBot() {
    auto enemy {m_am.createActor(Actor::Tag::BotPaddle)};
    auto shapeSize {enemy->addComponent<CShapeRectangle>(m_wm.relX(1), m_wm.relY(10), sf::Color::Red).shape.getSize()};
    const auto& transform {enemy->addComponent<CTransform>(
        sf::Vector2f{m_wm.relX(-20) - shapeSize.x, m_wm.centerOffsetY(-(shapeSize.y * 0.5))}, 
        m_wm.relY(100),
        sf::Vector2f{0, 0})
    };
    enemy->addComponent<CColliderBox>(transform.position, shapeSize);
    enemy->addComponent<CTextScore>(
        0,
        ResourceManager::Arcade,
        sf::Vector2f{m_wm.relX(75), m_wm.relY(1)},
        60,
        sf::Color::Red
    );
    enemy->addComponent<CAI>(0.35, 15);
}

void Engine::spawnPlayingNet() {
    const auto& renderSize {m_wm.renderSize()};
    auto net {m_am.createActor(Actor::Tag::Net)};
    auto& shape {net->addComponent<CShapeRectangle>(m_wm.relX(0.2), renderSize.y, sf::Color::White)};
    auto& transform {net->addComponent<CTransform>(sf::Vector2f{renderSize.x * 0.5, 0}, 0)};
    shape.shape.setPosition(transform.position);
}