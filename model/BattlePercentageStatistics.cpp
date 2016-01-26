#include <sstream>
#include "BattlePercentageStatistics.h"
#include "Player.h"

BattlePercentageStatistics::BattlePercentageStatistics(Statistics* statistics) {
    statistics_ = statistics;
}

BattlePercentageStatistics::~BattlePercentageStatistics() {
    delete statistics_;
}

std::string BattlePercentageStatistics::ToString(Player* mock) {
    std::ostringstream toPrint;
    toPrint << statistics_->ToString(mock);
    if (mock->GetBattlesCount() == 0) {
        toPrint << "\tPlayer hasn't participated in any battles" << std::endl;
    } else {
        toPrint << "\t% won battles: " << ((mock->GetBattlesWon() / (float)mock->GetBattlesCount()) * 100) << "%" << std::endl;
        toPrint << "\t% lost battles: " << (((mock->GetBattlesCount() - mock->GetBattlesWon()) / (float)mock->GetBattlesCount()) * 100) << "%" << std::endl;
    }
    return toPrint.str();
}