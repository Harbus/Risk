#pragma once
#include "Builder.h"
#include "RiskGame.h"

class GameBuilder : public Builder {
public:
    World* GetWorld(std::string fileName);
    std::vector<Player*> GetPlayersFromFile(std::string fileName);

    void SaveInProgress(RiskGame* game);
};