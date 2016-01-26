#pragma once
#include "Player.h"
class World;
class Statistics;

class RiskGame {
 public:
     RiskGame();
     ~RiskGame() = default;

     static RiskGame& Instance();
     void ShowMenu();
     void SetWorld(World* world);
     void SetPlayers(std::vector<Player*> players);
     void SetCard(int card, int amount) { deckOfCards_.at(card) = amount; };
     World* GetWorld();
     std::vector<Player*> GetPlayers();
     int GetCard(int card) { return deckOfCards_.at(card); };
     int GetTurnIns() { return numOfCardTurnIns_; };
     std::string GetMapFile(const std::string& extension);
     void SetTurnIns(int n) { numOfCardTurnIns_ = n; };
     void IncrementTurnIns() { numOfCardTurnIns_++; };
     void DecrementCard(int card) { deckOfCards_.at(card)--; };
     void IncrementCard(int card) { deckOfCards_.at(card)++; };
     bool IsRunning() const { return running_; }
     Statistics* GetStatistics() { return statistics_; }

 private:
     int numOfCardTurnIns_ = 0;
     std::vector<int> deckOfCards_ = { 12, 12, 12, 2 };
     std::string askExistingCountryName(World* world, const std::string message);
     std::string askExistingContinentName(World* world, const std::string message);
     void editMap(World* world);
     World* selectMap();
     void playMap(World* world, bool gameInProgress);
     World* world_;
     std::vector<Player*> players_;
     Statistics* statistics_;
     bool running_;
};