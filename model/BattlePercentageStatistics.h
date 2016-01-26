#pragma once
#include "Statistics.h"
class Player;

class BattlePercentageStatistics : public Statistics
{
public:
    BattlePercentageStatistics(Statistics* statistics);
    ~BattlePercentageStatistics();
    std::string ToString(Player* mock) override;

private:
    Statistics* statistics_;
};
