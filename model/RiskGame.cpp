#include <iostream>
#include <fstream>
#include <map>
#include "RiskGame.h"
#include "Country.h"
#include "Continent.h"
#include "World.h"
#include "PlayerList.h"
#include "Player.h"
#include "Phase.h"
#include "AttackPhase.h"
#include "FortificationPhase.h"
#include "ReinforcementPhase.h"
#include "StartupPhase.h"
#include "Utilities.h"
#include "World.h"
#include "GameBuilder.h"
#include "Director.h"
#include "BasicStatistics.h"
#include "WorldControlledPercentageStatistics.h"
#include "BattlePercentageStatistics.h"

RiskGame& RiskGame::Instance() {
    static RiskGame instance;
    return instance;
}

//constructor for RiskGame
RiskGame::RiskGame() {
    World::SaveToFile(World::CreateTestMap());
}

//display method for the main menu.
void RiskGame::ShowMenu() {
    running_ = true;
    int optionInput = -1;
    while (optionInput != 5) {
        std::cout << "~~~ Welcome to Risk COMP 345 ~~~" << std::endl;
        std::cout << "Select an action to make: " << std::endl;
        std::cout << "1. Create a map" << std::endl;
        std::cout << "2. Edit map" << std::endl;
        std::cout << "3. Play a new game" << std::endl;
        std::cout << "4. Load a game" << std::endl;
        std::cout << "5. Quit game" << std::endl;

        optionInput = -1;
        while (optionInput < 1 || optionInput > 5) {
            std::cout << "> ";
            std::cin >> optionInput;
            Utilities::ClearInputStream();
        }

        Utilities::ClearConsole();
        if (optionInput == 1) editMap(nullptr);
        else if (optionInput == 2) editMap(selectMap());
        else if (optionInput == 3) playMap(selectMap(), false);
        else if (optionInput == 4) {
            Director* director = new Director();
            GameBuilder* gameBuilder = new GameBuilder();
            director->SetBuilder(gameBuilder);
            director->LoadInProgress(GetMapFile(".save"));
            playMap(RiskGame::Instance().GetWorld(), true);
        }
        Utilities::ClearConsole();
    }
    std::cout << "Thank you for playing!";
    running_ = false;
}

//setter for world.
void RiskGame::SetWorld(World* world) {
    world_ = world;
}

//setter for players
void RiskGame::SetPlayers(std::vector<Player*> players) {
    players_ = players;
}


//getter for world
World* RiskGame::GetWorld() {
    return world_;
}

//getter for players
std::vector<Player*> RiskGame::GetPlayers() {
    return players_;
}

//method to check if a country already exists with the same name
std::string RiskGame::askExistingCountryName(World* world, const std::string message) {
    std::string countryName = "";
    std::cout << message << std::endl;
    do {
        std::cout << "> ";
        std::getline(std::cin, countryName);
    } while (!world->GetCountryByName(countryName));
    return countryName;
}

//method to check if a continent already exists with the same name
std::string RiskGame::askExistingContinentName(World* world, const std::string message) {
    std::string continentName = "";
    std::cout << message << std::endl;
    do {
        std::cout << "> ";
        std::getline(std::cin, continentName);
    } while (!world->GetContinentByName(continentName));
    return continentName;
}

//shortcut method for multiple uses.
//method asks the user to select a file from a list of files displayed.
std::string RiskGame::GetMapFile(const std::string& extension) {
    std::cout << "Select a map: " << std::endl;
    std::vector<std::string> mapNameList = Utilities::GetFilesInDirectory(".", extension);
    for (int i = 0; i < (int)mapNameList.size(); ++i) {
        std::cout << i << ". " << mapNameList.at(i) << std::endl;
    }

    int mapInput = -1;
    while (mapInput < 0 || mapInput >= (int)mapNameList.size()) {
        std::cout << "> ";
        std::cin >> mapInput;
    }
    std::cout << "Selected map " << mapNameList[mapInput] << "!" << std::endl << std::endl;

    return mapNameList[mapInput];
}

World* RiskGame::selectMap() {
    return  World::LoadFromFile(GetMapFile(".map"), false);
}

//main loop of the game, starts the game or continues the game depending on the boolean parameter.
void RiskGame::playMap(World* world, bool gameInProgress) {
    world_ = world;

    PlayerList* players = new PlayerList();
    std::vector<Phase*> phases = { new ReinforcementPhase(world, players),
                                   new AttackPhase(world, players),
                                   new FortificationPhase(world, players) };

    if (!gameInProgress) {
        StartupPhase startupPhase = StartupPhase(world, players);
        startupPhase.Start();
    }
    else {
        int i = 0;
        for (Player* player : RiskGame::Instance().GetPlayers()) {
            players->Add(player);
            if (player->GetTurn())
                players->SetCurrent(i);
            i++;
        }
    }
    statistics_ = new WorldControlledPercentageStatistics(new BattlePercentageStatistics(new BasicStatistics()));
    statistics_->AddPlayers(players);
    statistics_->Update();

    while (players->Size() > 1) {
        if (players->Current()->GetOwnedCountries().size() == 0) {
            std::cout << players->Current()->GetName() << " was removed from the game!" << std::endl;
            players->Remove(players->Current());
        }
        else {
            std::cout << "--- " << players->Current()->GetName() << "'s turn ---" << std::endl;
            for (Phase* phase : phases) {
                phase->Start();
            }
        }
        players->Current()->SetTurn(false);
        players->Next();
        players->Current()->SetTurn(true);
        std::cout << "Would you like to save game in progress? (y/n): " << std::endl;
        char inputAnswer = ' ';
        while (inputAnswer != 'y' && inputAnswer != 'n') {
            std::cout << "> ";
            std::cin >> inputAnswer;
        }
        if (inputAnswer == 'y') {
            Director* director = new Director();
            GameBuilder* gameBuilder = new GameBuilder();
            director->SetBuilder(gameBuilder);
            RiskGame* game = new RiskGame();
            game->SetWorld(GetWorld());
            game->SetPlayers(players->GetPlayers());
            gameBuilder->SaveInProgress(game);
        }
    }

    std::cout << players->At(0)->GetName() << " dominated the world!" << std::endl;
    std::cin.ignore();
    std::cin.ignore();

    delete world;
    delete players;
    for (int i = 0; i < 3; ++i) delete phases[i];
}

//edit a map method. the entirety of this method is to allow the user to create/edit components of an existing map
//this method will also make sure that the user creates a valid map, according to multiple standards.
void RiskGame::editMap(World* world) {
    if (!world) {
        std::string mapName;
        std::cout << "Name of new map file (.map): " << std::endl;
        std::cout << "> ";
        std::cin >> mapName;
        std::cout << std::endl;
        world = new World(mapName);
    }

    int userChoice = -1;
    while (userChoice != 10 && userChoice != 11) {
        std::cout << std::endl;
        std::cout << "Select an action to do to " << world->GetName() << ": " << std::endl;
        std::cout << "1. Show continents" << std::endl;
        std::cout << "2. Show countries" << std::endl;
        std::cout << "3. Add continent" << std::endl;
        std::cout << "4. Add country" << std::endl;
        std::cout << "5. Add connection" << std::endl;
        std::cout << "6. Remove continent" << std::endl;
        std::cout << "7. Remove country" << std::endl;
        std::cout << "8. Remove connection" << std::endl;
        std::cout << "9. Change map name" << std::endl;
        std::cout << "10. Save & return to menu" << std::endl;
        std::cout << "11. Return to menu" << std::endl;


        userChoice = -1;
        while (userChoice < 1 || userChoice > 11) {
            std::cout << "> ";
            std::cin >> userChoice;
        }
        std::cout << std::endl;

        if (userChoice == 1) {
            if (world->GetContinents().empty()) {
                std::cout << "No continents in this world!" << std::endl;
            }
            else {
                std::cout << "List of continents: " << std::endl;
                for (Continent* continent : world->GetContinents()) {
                    std::cout << "\t" << continent->GetName() << " : " << continent->GetCountries().size() << " countries" << std::endl;
                }
            }
        }
        else if (userChoice == 2) {
            if (world->GenerateCountryList().empty()) {
                std::cout << "No countries in this world!" << std::endl;
            }
            else {
                std::cout << "List of countries: " << std::endl;
                for (Country* country : world->GenerateCountryList()) {
                    std::cout << "\t" << country->GetName() << " : " << country->GetContinent()->GetName() << std::endl;
                }
            }
        }
        else if (userChoice == 3) {
            std::string continentName;
            std::cout << "Enter a name for the continent: " << std::endl << "> ";
            Utilities::ClearInputStream();
            std::getline(std::cin, continentName);
            int controlValue;
            std::cout << "Enter a control value for the continent: " << std::endl << "> ";
            std::cin >> controlValue;
            std::cout << "New continent " << continentName << " created with a control value of " << controlValue << "!" << std::endl;
            world->AddContinent(new Continent(continentName, controlValue));
        }
        else if (userChoice == 4) {
            std::string continentName, countryName;
            std::cout << "Enter a name for the country: " << std::endl << "> ";
            Utilities::ClearInputStream();
            std::getline(std::cin, countryName);
            continentName = askExistingContinentName(world, "Enter the name of the continent this country belongs to: ");
            std::cout << "New country " << countryName << " created and added to " << continentName << "!" << std::endl;
            world->GetContinentByName(continentName)->AddCountry(new Country(countryName));
        }
        else if (userChoice == 5) {
            std::string keepConnecting = "y";
            while (keepConnecting == "y") {
                std::string connectingCountry, countryName;
                Utilities::ClearInputStream();
                countryName = askExistingCountryName(world, "Enter the name of the country you wish to add a connection to: ");
                connectingCountry = askExistingCountryName(world, "Enter the name of the country you wish to connect to: ");
                Country::Connect(world->GetCountryByName(countryName), world->GetCountryByName(connectingCountry));
                std::cout << countryName << " and " << connectingCountry << " are now connected!" << std::endl;
                std::cout << "Do you wish to add another connection? (y/n) ";
                std::cin >> keepConnecting;
            }
        }
        else if (userChoice == 6) {
            std::string continentName;
            Utilities::ClearInputStream();
            continentName = askExistingContinentName(world, "Enter the name of the continent you wish to remove: ");
            world->RemoveContinent(world->GetContinentByName(continentName));
            std::cout << continentName << " has been removed successfully." << std::endl;
        }
        else if (userChoice == 7) {
            std::string countryName;
            Utilities::ClearInputStream();
            countryName = askExistingCountryName(world, "Enter the name of the country you wish to remove: ");
            world->RemoveCountry(world->GetCountryByName(countryName));
            std::cout << countryName << " has been removed successfully." << std::endl;
        }
        else if (userChoice == 8) {
            std::string keepDisconnecting = "y";
            while (keepDisconnecting == "y") {
                std::string disconnectingCountry, countryName;
                Utilities::ClearInputStream();
                countryName = askExistingCountryName(world, "Enter a country to disconnect: ");
                disconnectingCountry = askExistingCountryName(world, "Enter country you wish to disconnect from: ");
                Country::Connect(world->GetCountryByName(countryName), world->GetCountryByName(disconnectingCountry));
                std::cout << countryName << " and " << disconnectingCountry << " have been disconnected!" << std::endl << std::endl;
                std::cout << "Do you wish to remove another connection? (y/n) ";
                std::cin >> keepDisconnecting;
            }
        }
        else if (userChoice == 9) {
            std::string newFileName;
            std::cout << "Enter the new name for your map: " << std::endl << "> ";
            std::cin >> newFileName;
            world->SetName(newFileName);
        }
        else if (userChoice == 10) {
            int errorID = world->CheckCorrecteness();
            if (errorID == 0) {
                World::SaveToFile(world);
                std::cout << world->GetName() << " was saved! Press \"ENTER\" to continue ... " << std::endl;
                delete world;
                std::cin.ignore();
                std::cin.ignore();
            }
            else {
                std::cout << World::ErrorToString(errorID) << std::endl;
                userChoice = -1;
            }
        }
        else if (userChoice == 11) {
            delete world;
        }
    }
}