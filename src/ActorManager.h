#ifndef ACTOR_MANAGER_H
#define ACTOR_MANAGER_H
#include "Actor.h"
#include <cstddef>
#include <vector>
#include <memory>
#include <unordered_map>

using ActorStore = std::vector<std::shared_ptr<Actor>>;
using TaggedActorStore = std::unordered_map<Actor::Tag, ActorStore>;

class ActorManager {
private:
    ActorStore m_actors {};
    TaggedActorStore m_taggedActors {};
    ActorStore m_addBufferActors {};
    std::size_t m_nextId {0};

public:
    std::shared_ptr<Actor> createActor(Actor::Tag tag);

    ActorStore& getActors();
    ActorStore& getActors(Actor::Tag tag);

    void update();
};

#endif