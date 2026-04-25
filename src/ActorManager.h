#ifndef ACTOR_MANAGER_H
#define ACTOR_MANAGER_H
#include "Actor.h"
#include <cstddef>          // std::size_t
#include <vector>           // std::vector
#include <memory>           // std::shared_ptr
#include <algorithm>        // std::erase_if, std::ranges::for_each
#include <utility>          // std::pair

using ActorStore = std::vector<std::shared_ptr<Actor>>;
using TaggedActorStore = std::unordered_map<Actor::Tag, ActorStore>;

class ActorManager {
private:
    ActorStore m_actors {};
    TaggedActorStore m_taggedActors {};
    ActorStore m_addBufferActors {};
    std::size_t m_nextId {0};

public:
    std::shared_ptr<Actor> createActor(Actor::Tag tag) {
        auto a {std::shared_ptr<Actor>{new Actor{m_nextId++, tag}}};
        m_actors.push_back(a);
        m_taggedActors[tag].push_back(a);
        return a;
    }

    void update() {
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

};

#endif