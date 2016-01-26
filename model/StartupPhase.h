#pragma once
#include "Phase.h"

class StartupPhase : public Phase {
 public:
    StartupPhase(World* world, PlayerList* players);
    virtual ~StartupPhase() = default;
    void Start() override;

 private:
    void chooseNumberOfPlayers();
    void assignCountriesToPlayers();
};
