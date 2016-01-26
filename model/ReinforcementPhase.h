#pragma once
#include "Phase.h"

class ReinforcementPhase : public Phase {
 public:
    ReinforcementPhase(World* world, PlayerList* players);
    virtual ~ReinforcementPhase() = default;
    void Start() override;

 private:
    int turnInCards();
    int calculateReinforcements();
    void placeReinforcements(int reinforcements);
};
