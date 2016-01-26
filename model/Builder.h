#pragma once
#include <vector>
#include "Player.h"

class World;

class Builder {
public:
    virtual World* GetWorld(std::string fileName) =  0;
    virtual std::vector<Player*> GetPlayersFromFile(std::string fileName) = 0;
};