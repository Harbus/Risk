#include "GameBuilder.h"
#include "World.h"
#include "Country.h"
#include "Continent.h"
#include <iostream>
#include <sstream>
#include <fstream>

World* GameBuilder::GetWorld(std::string fileName) {
    std::ifstream in(fileName);
    World* world = new World(fileName);
    std::string line;
    //Load all information of the world
    while (std::getline(in, line)) {
        if (line.find("[World]") != std::string::npos) {
            int numTurnIns = atoi(line.substr(10).c_str());
            RiskGame::Instance().SetTurnIns(numTurnIns);
            for (int i = 0; i < 4; i++) {
                int numOfCard = atoi(line.substr(13 + (i * 2)).c_str());
                RiskGame::Instance().SetCard(i, numOfCard);
            }
            world = World::LoadFromFile(fileName, true);
        }
    }
    return world;
}

std::vector<Player*> GameBuilder::GetPlayersFromFile(std::string fileName) {
    std::ifstream in(fileName);
    std::vector<Player*> players;
    std::string line;
    //Load all information of players and stops once world properties have been reach
    while (std::getline(in, line)) {
        if (line.find("[Players]") != std::string::npos) {
            continue;
        }
        if (line.find("[World]") != std::string::npos)
            break;
        //Players name
        int playerNameStart = (int)line.find("\"") + 1;
        int playerNameEnd = (int)line.find("\"", playerNameStart) - 1;
        std::string playerName = line.substr(playerNameStart, playerNameEnd - playerNameStart + 1);
        //Check if its their turn
        int turnStart = playerNameEnd + 3;
        int turn = atoi(line.substr(turnStart).c_str());
        int playerCardsStart = turnStart + 3;
        Player* player = new Player(playerName);
        if (turn == 1) {
            player->SetTurn(true);
        }
        else {
            player->SetTurn(false);
        }
        //Load cards that player owned
        for (int i = 0; i < 4; i++) {
            int currentCard = atoi(line.substr(playerCardsStart).c_str());
            player->SetCard(i, currentCard);
            playerCardsStart += 2;
        }
        players.push_back(player);
    }
    return players;
}

void GameBuilder::SaveInProgress(RiskGame* game) {
    World* world = game->GetWorld();
    std::ofstream out(world->GetName().substr(0, world->GetName().find(".")) + ".save");
    //Save all of players properties (Cards/Turn)
    out << "[Players]" << std::endl;
    for (Player* player : game->GetPlayers()) {
        out << "\"" << player->GetName() << "\" ";
        if (player->GetTurn()) {
            out << "1";
        }
        else {
            out << "0";
        }
        out << " [";
        for (int i = 0; i < 4; i++) {
            if (i == 3)
                out << player->GetCard(i);
            else
                out << player->GetCard(i) << ",";
        }
        out << "]" << std::endl;
    }
    //Save all of worlds properties (Countries/Continents/Owner of country, etc..)
    out << "[World] " << RiskGame::Instance().GetTurnIns() << " [";
    for (int i = 0; i < 4; i++) {
        if (i == 3)
            out << RiskGame::Instance().GetCard(i);
        else
            out << RiskGame::Instance().GetCard(i) << ",";
    }
    out << "]" << std::endl;

    for (Continent* continent : world->GetContinents()) {
        out << "continent \"" << continent->GetName() << "\" ";
        out << continent->GetControlValue() << std::endl;
    }

    for (Country* country : world->GenerateCountryList()) {
        out << "country \"" << country->GetName() << "\" ";
        out << "\"" << country->GetContinent()->GetName() << "\" ";
        out << "\"" << country->GetOwner()->GetName() << "\" ";
        out << country->GetArmy() << std::endl;
    }

    for (Country* country : world->GenerateCountryList()) {
        out << "connection \"" << country->GetName() << "\" [";
        for (Country* connection : country->GetConnections()) {
            if (!connection->GetConnections().empty()) {
                out << connection->GetName() << ",";
            }
        }
        out << "]" << std::endl;
    }
    out.close();
}