#include <sstream>
#include "BasicStatistics.h"
#include "Player.h"

std::string BasicStatistics::ToString(Player* mock) {
    std::ostringstream toPrint;
    toPrint << mock->GetName().c_str() << std::endl;
    toPrint << "\tCountries owned: " << mock->GetOwnedCountries().size() << std::endl;
    toPrint << "\t# of armies owned: " << mock->ComputeTotalNumberOfArmiesOwned() << std::endl;
    toPrint << "\t# of cards owned: " << mock->GetCardCount() << std::endl;
    return toPrint.str();
}