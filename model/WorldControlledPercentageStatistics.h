#pragma once
#include "Statistics.h"
class Player;

class WorldControlledPercentageStatistics : public Statistics
{
public:
    WorldControlledPercentageStatistics(Statistics* statistics);
    ~WorldControlledPercentageStatistics();
    std::string ToString(Player* mock) override;

private:
    Statistics* statistics_;
};

