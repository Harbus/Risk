#include <sstream>
#include "WorldControlledPercentageStatistics.h"
#include "Player.h"
#include "World.h"
#include "RiskGame.h"

WorldControlledPercentageStatistics::WorldControlledPercentageStatistics(Statistics* statistics) {
    statistics_ = statistics;
    RiskGame::Instance().GetWorld()->Attach(this);
}

WorldControlledPercentageStatistics::~WorldControlledPercentageStatistics() {
    delete statistics_;
}

std::string WorldControlledPercentageStatistics::ToString(Player* mock) {
    std::ostringstream toPrint;
    toPrint << statistics_->ToString(mock);
    toPrint << "\t% world controlled: " << (mock->GetOwnedCountries().size()/(float)RiskGame::Instance().GetWorld()->GenerateCountryList().size())*100 << "%" << std::endl;
    return toPrint.str();
}
