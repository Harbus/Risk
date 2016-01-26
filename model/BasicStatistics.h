#pragma once
#include "Statistics.h"
class Player;

class BasicStatistics : public Statistics
{
public:
    BasicStatistics() = default;
    virtual ~BasicStatistics() = default;
    std::string ToString(Player* mock) override;
};

