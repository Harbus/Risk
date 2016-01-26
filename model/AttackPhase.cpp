#include <iostream>
#include <vector>
#include <algorithm>
#include "AttackPhase.h"
#include "PlayerList.h"
#include "Player.h"
#include "Country.h"
#include "Dice.h"
#include "World.h"
#include "RiskGame.h"

AttackPhase::AttackPhase(World* world, PlayerList* players)
    : Phase(world, players) {
}

void AttackPhase::Start() {
    std::cout << "\t| Attack Phase" << std::endl;
    Country* attackingCountry;
    //While the user has a country to attack with, keep asking him if he wants to attack
    do {
        attackingCountry = chooseAttackingCountry();
        if (!attackingCountry) {
            std::cout << "You have stopped attacking countries!" << std::endl;
            continue;
        }
        Country* defendingCountry = chooseDefendingCountry(attackingCountry);

        int attackingArmies = chooseAttackArmies(attackingCountry);
        if (attackCountry(attackingCountry, defendingCountry, attackingArmies)) {
            chooseSendArmies(attackingCountry, defendingCountry, attackingArmies);
        }
    } while (attackingCountry != nullptr);
}

Country* AttackPhase::chooseAttackingCountry() {
    int choosableForAttackingCountryCount = 0;
    const std::vector<Country*>& ownedCountries = players_->Current()->GetOwnedCountries();
    //Select which country you wish to use for attack
    for (int i = 0; i < (int)ownedCountries.size(); ++i) {
        int armyCount = ownedCountries.at(i)->GetArmy();
        int connectionsCount = (int)ownedCountries.at(i)->GetPlayerNotOwnedConnections(players_->Current()).size();
        if (connectionsCount > 0 && armyCount > 1) {
            std::cout << "\t\t" << i << ". " << ownedCountries.at(i)->GetName() << " : " << ownedCountries.at(i)->GetArmy() << " armies : "
                << connectionsCount << " enemy connections" << std::endl;
            choosableForAttackingCountryCount++;
        }
    }
    std::cout << "\t\t" << (int)ownedCountries.size() << ". " << "None" << std::endl;

    if (choosableForAttackingCountryCount == 0) {
        std::cout << "You have no countries from which you can attack from!" << std::endl;
        return nullptr;
    }

    std::cout << "Enter the country index that you wish to use to attack: " << std::endl;
    int inputIndex = -1;
    while (inputIndex < 0 || inputIndex >= (int)ownedCountries.size()) {
        std::cout << "> ";
        std::cin >> inputIndex;
        if (inputIndex == (int)ownedCountries.size()) return nullptr;
    }

    return ownedCountries.at(inputIndex);
}

Country* AttackPhase::chooseDefendingCountry(Country* attackingCountry) {
    const std::vector<Country*>& connectedEnemyCountries = attackingCountry->GetPlayerNotOwnedConnections(players_->Current());
    for (size_t i = 0; i < connectedEnemyCountries.size(); ++i) {
        std::cout << "\t\t" << i << ". " << connectedEnemyCountries.at(i)->GetName() << " : " 
            << connectedEnemyCountries.at(i)->GetArmy() << " armies : " 
            << "owned by " << connectedEnemyCountries.at(i)->GetOwner()->GetName() << std::endl;
    }
    //Select country you wish to attack
    std::cout << "Enter the enemy country index that you wish to attack: " << std::endl;
    int inputIndex = -1;
    while (inputIndex < 0 || inputIndex >= (int)connectedEnemyCountries.size()) {
        std::cout << "> ";
        std::cin >> inputIndex;
    }

    return connectedEnemyCountries.at(inputIndex);
}

int AttackPhase::chooseAttackArmies(Country* attackingCountry) {
    //Select armies you want to send to attack country
    std::cout << "Choose the amount of armies to send to the fight (max: " << attackingCountry->GetArmy() - 1 << "): " << std::endl;
    std::string input;
    int chosenAttackingArmies = 0;
    while (chosenAttackingArmies <= 0 || chosenAttackingArmies > attackingCountry->GetArmy() - 1) {
        std::cout << "> ";
        std::cin >> input;
        chosenAttackingArmies = atoi(input.c_str());
    }
    attackingCountry->IncrementArmy(-chosenAttackingArmies);
    return chosenAttackingArmies;
}

bool AttackPhase::attackCountry(Country* attackingCountry, Country* defendingCountry, int& attackingArmies) {
    attackingCountry->GetOwner()->IncrementBattlesCount();
    defendingCountry->GetOwner()->IncrementBattlesCount();

    bool allIn = false;
    bool continueFighting = true;
    bool receivedCard = false;
    //Attacking algorithm. Keeps going until either armies have 0 soldiers, or attacker stops attacking
    while (attackingArmies > 0 && defendingCountry->GetArmy() > 0 && continueFighting) {
        int minAttackingArmies = std::min(3, attackingArmies);
        int minDefendingArmies = std::min(2, defendingCountry->GetArmy());
        int minCompareRolls = std::min(minAttackingArmies, minDefendingArmies);

        int attackingRolls[3] = { 0, 0, 0 };
        for (int i = 0; i < minAttackingArmies; ++i) {
            attackingRolls[i] = Dice::Roll();
        }

        int defendingRolls[2] = { 0, 0 };
        for (int i = 0; i < minDefendingArmies; ++i) {
            defendingRolls[i] = Dice::Roll();
        }

        std::sort(std::begin(attackingRolls), std::end(attackingRolls));
        std::reverse(std::begin(attackingRolls), std::end(attackingRolls));
        std::cout << "Attacker rolled [ ";
        for (int i = 0; i < minAttackingArmies; ++i) {
            std::cout << attackingRolls[i] << " ";
        }
        std::cout << "]" << std::endl;

        std::sort(std::begin(defendingRolls), std::end(defendingRolls));
        std::reverse(std::begin(defendingRolls), std::end(defendingRolls));
        std::cout << "Defender rolled [ ";
        for (int i = 0; i < minDefendingArmies; ++i) {
            std::cout << defendingRolls[i] << " ";
        }
        std::cout << "]" << std::endl;

        for (int i = 0; i < minCompareRolls; ++i) {
            if (defendingRolls[i] >= attackingRolls[i]) {
                attackingArmies--;
                std::cout << "Attacker loses an army! Now at: " << attackingArmies << std::endl;
            }
            else {
                defendingCountry->IncrementArmy(-1);
                std::cout << "Defender loses an army! Now at: " << defendingCountry->GetArmy() << std::endl;
            }
        }
        std::cout << std::endl;

        if (!allIn && attackingArmies > 0 && defendingCountry->GetArmy() > 0) {
            std::cout << "Do you wish to continue fighting (y/n, a for all in)?" << std::endl;
            std::string input;
            do {
                std::cout << "> ";
                std::cin >> input;
            } while (input != "n" && input != "y" && input != "a");

            if (input == "n") {
                continueFighting = false;
            }
            else if (input == "a") {
                allIn = true;
            }
            std::cout << std::endl;
        }
    }

    if (defendingCountry->GetArmy() == 0) {
        std::cout << "Attacker won with " << attackingArmies << " armies left!" << std::endl;
        attackingCountry->IncrementArmy(attackingArmies);
        defendingCountry->GetOwner()->RemoveOwnedCountry(defendingCountry);
        players_->Current()->AddOwnedCountry(defendingCountry);
        if (!receivedCard) {
            getCard();
            receivedCard = true;
        }
        attackingCountry->GetOwner()->IncrementBattlesWon();
        return true;
    } else if (attackingArmies == 0) {
        std::cout << "Defender won with " << defendingCountry->GetArmy() << " armies left!" << std::endl;
        defendingCountry->GetOwner()->IncrementBattlesWon();
    } else {
        std::cout << "The battle was aborted; Attacker has " << attackingArmies << " attacking armies left, Defender has " << defendingCountry->GetArmy() << " armies left." << std::endl;
        attackingCountry->IncrementArmy(attackingArmies);
    }
    return false;
}

//Decide how many armies Attacker wants to send to conquered country
void AttackPhase::chooseSendArmies(Country* attackingCountry, Country* conqueredCountry, int attackingArmies) {
    if (attackingCountry->GetArmy() > 1) {
        int minArmySend = std::min(attackingArmies, attackingCountry->GetArmy() - 1);
        std::cout << "How many armies do you want to send to the conquered territory (min: " << minArmySend << " max: " << attackingCountry->GetArmy() - 1 << ")?" << std::endl;
        int sentArmies = 0;
        do {
            std::cout << "> ";
            std::string input;
            std::cin >> input;
            sentArmies = atoi(input.c_str());
        } while (sentArmies < minArmySend || sentArmies > attackingCountry->GetArmy() - 1);

        attackingCountry->IncrementArmy(-sentArmies);
        conqueredCountry->IncrementArmy(sentArmies);
        std::cout << "Sent " << sentArmies << " armies from " << attackingCountry->GetName() << " to " << conqueredCountry->GetName() << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << "You do not have enough armies to send to the conquered territory." << std::endl;
    }
}
//If attacker conquers country, he gets a card from deck
void AttackPhase::getCard() {
    int randomCard = rand() % 4;
    while (RiskGame::Instance().GetCard(randomCard) == 0)
        randomCard = rand() % 4;
    players_->Current()->AddCard(randomCard);
}