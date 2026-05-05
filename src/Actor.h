#ifndef ACTOR_H
#define ACTOR_H
#include <cstddef>
#include <tuple>
#include <unordered_map>
#include <typeinfo>
#include <type_traits>
#include <utility>
#include <concepts>
#include "Components.h"

using Components = std::tuple<
    CTransform, 
    CShapeCircle,
    CShapeRectangle,
    CHealth, 
    CColliderBox,
    CColliderCircle, 
    CInput, 
    CAI,
    CTextScore
>;

class Actor {
public:
    enum Tag {
        PlayerPaddle,
        BotPaddle,
        Score,
        Ball,
        Net
    };

private:
    std::size_t m_id {};
    Tag m_tag {};
    bool m_exists {true};
    Components m_components {};

    Actor(std::size_t id, Tag tag) : m_id {id}, m_tag {tag} {} // private, for only the factory ActorManger constructs actors
    
public:
    friend class ActorManager;
    
    template <typename C, typename... CArgs> requires std::derived_from<C, Component> && std::constructible_from<C, CArgs...>
    C& addComponent(CArgs&&... componentArgs) {
        C& component {getComponent<C>()};
        component = C(std::forward<CArgs>(componentArgs)...);
        component.exists = true;
        return component;
    }

    template <typename C> requires std::derived_from<C, Component>
    C& getComponent() {
        return std::get<C>(m_components);
    }

    template <typename C> requires std::derived_from<C, Component>
    const C& getComponent() const {
        return std::get<C>(m_components);
    }

    template <typename C> requires std::derived_from<C, Component>
    void remove() {
        std::get<C>(m_components) = C{}; // default construction would set 'exists' to FALSE and default-set other fields (not considered part of actor)
    }

    template <typename C> requires std::derived_from<C, Component>
    bool hasComponent() const {
        return std::get<C>(m_components).exists;
    }

    bool exists() const {
        return m_exists;
    }

    std::size_t id() const {
        return m_id;
    }

    Tag tag() const {
        return m_tag;
    }

    void destroy() {
        m_exists = false;
    }
};

#endif