#pragma once
#include "Phase.h"
class Player;

class FortificationPhase : public Phase {
 public:
    FortificationPhase(World* world, PlayerList* players);
    virtual ~FortificationPhase() = default;
    void Start() override;

 private:
    bool wishesToFortify();
    void chooseFortification();
};

