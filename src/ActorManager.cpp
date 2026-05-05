#include "ActorManager.h"
#include "Actor.h"
#include <memory>
#include <algorithm>
#include <vector>
#include <utility>
#include <unordered_map>

std::shared_ptr<Actor> ActorManager::createActor(Actor::Tag tag) {
    auto a {std::shared_ptr<Actor>{new Actor{m_nextId++, tag}}};
    m_actors.push_back(a);
    m_taggedActors[tag].push_back(a);
    return a;
}

ActorStore& ActorManager::getActors() {
    return m_actors;
}

ActorStore& ActorManager::getActors(Actor::Tag tag) {
    return m_taggedActors[tag];
}

void ActorManager::update() {
    static const auto toDeleteActors {[](const std::shared_ptr<Actor>& a) { return !a->exists(); }};
    static const auto toDeleteTaggedActors {[](std::pair<const Actor::Tag, ActorStore>& taggedA) { std::erase_if(taggedA.second, toDeleteActors); }};

    std::erase_if(m_actors, toDeleteActors);
    std::for_each(m_taggedActors.begin(), m_taggedActors.end(), toDeleteTaggedActors);

    for (const auto& a : m_addBufferActors) {
        m_actors.push_back(a);
        m_taggedActors[a->tag()].push_back(a);
    }

    m_addBufferActors.clear();
}