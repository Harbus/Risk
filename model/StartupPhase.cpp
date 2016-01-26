#include <algorithm>
#include <iostream>
#include "StartupPhase.h"
#include "World.h"
#include "Player.h"
#include "PlayerList.h"
#include "Country.h"

StartupPhase::StartupPhase(World* world, PlayerList* players)
    : Phase(world, players) {
}

void StartupPhase::Start() {
    std::cout << "| Startup Phase" << std::endl;
    chooseNumberOfPlayers();
    assignCountriesToPlayers();
}

void StartupPhase::chooseNumberOfPlayers() {
    std::cout << "Enter # of players: " << std::endl;
    int inputAmountOfPlayers = -1;
    //Choosing number of players, max of 6
    while (inputAmountOfPlayers <= 1 || inputAmountOfPlayers > 6) {
        std::cout << "> ";
        std::cin >> inputAmountOfPlayers;
    }
    //Create new players
    for (int i = 0; i < inputAmountOfPlayers; ++i) {
        players_->Add(new Player("Player " + std::to_string(i + 1)));
    }
}

//Assign countries randomly to all players
void StartupPhase::assignCountriesToPlayers() {
    std::vector<Country*> countries = world_->GenerateCountryList();
    std::random_shuffle(countries.begin(), countries.end());

    int startArmies = 35 - ((players_->Size() - 3) * 5);
    int averageCountryPerPlayer = (int)(countries.size() / players_->Size());

    for (size_t i = 0; i < countries.size(); ++i) {
        players_->At(i % players_->Size())->AddOwnedCountry(countries.at(i));
        countries.at(i)->SetArmy((int)(startArmies / averageCountryPerPlayer));
    }
}
