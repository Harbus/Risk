#include <iostream>
#include "FortificationPhase.h"
#include "Country.h"
#include "PlayerList.h"
#include "Player.h"

FortificationPhase::FortificationPhase(World* world, PlayerList* players)
    : Phase(world, players) {
}

void FortificationPhase::Start() {
    std::cout << "\t| Fortification Phase" << std::endl;

    if (wishesToFortify()) {
        chooseFortification();
    }
}
//Check if player wants to fortify
bool FortificationPhase::wishesToFortify() {
    std::cout << "Do you wish to fortify? (y/n): " << std::endl;
    char inputAnswer = ' ';
    while (inputAnswer != 'y' && inputAnswer != 'n') {
        std::cout << "> ";
        std::cin >> inputAnswer;
    }

    if (inputAnswer == 'n') {
        return false;
    }
    return true;
}

void FortificationPhase::chooseFortification() {
    int fortifiableCountryCount = 0;
    const std::vector<Country*>& ownedCountries = players_->Current()->GetOwnedCountries();
    //Lists countries that you can fortify
    for (int i = 0; i < (int)ownedCountries.size(); ++i) {
        int armyCount = ownedCountries.at(i)->GetArmy();
        int connectionsCount = (int)ownedCountries.at(i)->GetEmptyAndPlayerOwnedConnections(players_->Current()).size();
        if (connectionsCount > 0 && armyCount > 1) {
            std::cout << "\t\t" << i << ". " << ownedCountries.at(i)->GetName() << " : " << ownedCountries.at(i)->GetArmy() << " armies : "
                << connectionsCount << " connections" << std::endl;
            fortifiableCountryCount++;
        }
    }

    if (fortifiableCountryCount == 0) {
        std::cout << "You have no countries from which you can move armies!" << std::endl;
        return;
    }

    //Asks which country player wishes to reinforce
    std::cout << "Enter the country index from which you want to move armies: " << std::endl;
    int inputSourceIndex = -1;
    while (inputSourceIndex < 0 || inputSourceIndex >= (int)ownedCountries.size()) {
        std::cout << "> ";
        std::cin >> inputSourceIndex;
    }

    Country* sourceCountry = ownedCountries.at(inputSourceIndex);

    //Asks how many armies to move
    int sourceArmyCount = sourceCountry->GetArmy();
    std::cout << "Enter the # of armies to send (max " << sourceArmyCount - 1 << "): " << std::endl;
    int inputArmies = -1;
    while (inputArmies <= 0 || inputArmies >= sourceArmyCount) {
        std::cout << "> ";
        std::cin >> inputArmies;
    }

    const std::vector<Country*>& connectedCountries = ownedCountries.at(inputSourceIndex)->GetEmptyAndPlayerOwnedConnections(players_->Current());
    for (size_t i = 0; i < connectedCountries.size(); ++i) {
        std::cout << "\t\t" << i << ". " << connectedCountries.at(i)->GetName() << " : "
            << connectedCountries.at(i)->GetArmy() << " armies" << std::endl;
    }
    //Asks which country you wish to move armies
    std::cout << "Enter the connected country index of where you want to move the armies: " << std::endl;
    int inputDestinationIndex = -1;
    while (inputDestinationIndex < 0 || inputDestinationIndex >= (int)connectedCountries.size()) {
        std::cout << "> ";
        std::cin >> inputDestinationIndex;
    }
    //Send armies to new country and remove armies from previous country
    Country* destinationCountry = connectedCountries.at(inputDestinationIndex);
    sourceCountry->IncrementArmy(-inputArmies);
    destinationCountry->IncrementArmy(inputArmies);
    if (!destinationCountry->GetOwner()) {
        players_->Current()->AddOwnedCountry(destinationCountry);
    }

    std::cout << "You have moved " << inputArmies << " armies from " << sourceCountry->GetName() << " to " << destinationCountry->GetName() << "!" << std::endl;
}