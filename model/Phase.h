#pragma once
#include <vector>
class World;
class PlayerList;

class Phase {
 public:
    Phase(World* world, PlayerList* players);
    virtual ~Phase() = default;
    virtual void Start() = 0;

 protected:
    World* world_;
    PlayerList* players_;
};
